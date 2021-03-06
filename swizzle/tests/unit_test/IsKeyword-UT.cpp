#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/types/IsKeyword.hpp>

namespace {

    using namespace swizzle;
    using namespace swizzle::types;
    
    TEST(verifyIsKeyword)
    {
        CHECK(IsKeyword("case"));
        CHECK(IsKeyword("const"));
        CHECK(IsKeyword("enum"));
        CHECK(IsKeyword("extern"));
        CHECK(IsKeyword("import"));
        CHECK(IsKeyword("namespace"));

        CHECK(IsKeyword("struct"));
        CHECK(IsKeyword("using"));

        CHECK(!IsKeyword("bitfield"));
        CHECK(!IsKeyword("u8"));
        CHECK(!IsKeyword("i8"));
        CHECK(!IsKeyword("u16"));
        CHECK(!IsKeyword("i16"));
        CHECK(!IsKeyword("u32"));
        CHECK(!IsKeyword("i32"));
        CHECK(!IsKeyword("u64"));
        CHECK(!IsKeyword("i64"));

        CHECK(!IsKeyword("variable_block"));
    }
}
