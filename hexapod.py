#!/usr/bin/env python3

from cffi import FFI
ffi = FFI()

# Create interface for FFI
# Note that this must be manually updated :-/
ffi.cdef("""
   struct hexapod_config_s {
        float length;
        float width;
        float offset_a;
        float len_ab;
        float len_bc;
    };

    struct hexapod_s {
        struct hexapod_config_s config;
    };

    struct hpod_vector3_s {
        float x;
        float y;
        float z;
    };

    struct hpod_gait_s {
        struct hpod_vector3_s movement;
        struct hpod_vector3_s offset;
        float height_scale;
    };

    void HPOD_init(struct hexapod_s* hexapod, struct hexapod_config_s* config);
    int HPOD_leg_ik3(struct hexapod_s* hexapod, struct hpod_vector3_s *end_pos,
                  float* alpha, float* beta, float* theta);
    void HPOD_gait_calc(struct hexapod_s* hexapod, struct hpod_gait_s *gait, struct hpod_vector3_s *movement,
                    float phase_scl, struct hpod_vector3_s* leg_pos);
    int HPOD_gait_valid(struct hexapod_s* hexapod, struct hpod_gait_s *gait);
""")

# Open library
lib = ffi.dlopen("./build/libhexapod.dylib")

# Hexapod wrapper class
class Hexapod:
    def __init__(self, length=300, width=140, offset_a=50, len_ab=80, len_bc=150):
        self.length = length
        self.width = width
        self.offset_a = offset_a
        self.len_ab = len_ab
        self.len_bc = len_bc

        # Create default hexapod
        config = ffi.new("struct hexapod_config_s *", (length, width, offset_a, len_ab, len_bc))
        self.hexy = ffi.new("struct hexapod_s *")
        lib.HPOD_init(self.hexy, config)

        # Set default gait
        self.set_gait()

    def set_gait(self, movement=(100.0, 200.0, 40.0), offset=(150.0, 0.0, -100.0), lift=0.05):
        self.gait = ffi.new("struct hpod_gait_s *", (movement, offset, lift))

    def leg_ik3(self, x, y, z):
        end_pos = ffi.new("struct hpod_vector3_s *", (x, y, z))
        alpha = ffi.new("float*")
        beta = ffi.new("float*")
        theta = ffi.new("float*")
        lib.HPOD_leg_ik3(self.hexy, end_pos, alpha, beta, theta)
        return (float(alpha[0]), float(beta[0]), float(theta[0]))

    def gait_calc(self, movement, phase):
        m = ffi.new("struct hpod_vector3_s *", movement)
        pos = ffi.new("struct hpod_vector3_s *")
        lib.HPOD_gait_calc(self.hexy, self.gait, m, phase, pos)
        return (float(pos.x), float(pos.y), float(pos.z))

    def gait_valid(self):
        res = lib.HPOD_gait_valid(self.hexy, self.gait)
        return (res == 0)

# Run tests if loaded directly
if __name__ == "__main__":
    print("libhexapod python bindings")
    h = Hexapod()
    print("Testing IK3")
    print(h.leg_ik3(100.0, 100.0, 100.0))
    print("Testing Gait Calculation")
    print(h.gait_calc((100.0, 100.0, 40.0), 0.0))
    print("Testing Gait Validity")
    print(h.gait_valid())
    print("Tests complete")



