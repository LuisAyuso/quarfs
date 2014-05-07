

#include "gtest/gtest.h"

#include "render/window_mgr.h"
#include "world/world.h"

TEST(FactorialTest, Negative) {
	EXPECT_EQ(1, 1);
    WindowManager window(100,100, "test");

    World w = WorldFactory::getEmptyWorld();

	std::cout << w << std::endl;
}
