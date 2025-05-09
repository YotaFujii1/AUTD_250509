#include <gtest/gtest.h>

#include <autd3/gain/group.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(Gain, Group) {
  auto autd = create_controller();

  const auto cx = autd.center().x();

  autd.send(autd3::gain::Group([cx](const auto&) {
              return [cx](const auto& tr) -> std::optional<const char*> {
                if (tr.position().x() < cx) return "uniform";
                return "null";
              };
            })
                .set("uniform", autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90)))
                .set("null", autd3::gain::Null()));
  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (auto& tr : dev) {
      if (tr.position().x() < cx) {
        ASSERT_EQ(0x80, drives[tr.idx()].intensity.value());
        ASSERT_EQ(0x90, drives[tr.idx()].phase.value());
      } else {
        ASSERT_EQ(0, drives[tr.idx()].intensity.value());
        ASSERT_EQ(0, drives[tr.idx()].phase.value());
      }
    }
  }

  autd.send(autd3::gain::Group([cx](const auto&) {
              return [cx](const auto& tr) -> std::optional<const char*> {
                if (tr.position().x() < cx) return "uniform";
                return std::nullopt;
              };
            }).set("uniform", autd3::gain::Uniform(autd3::driver::EmitIntensity(0x81), autd3::driver::Phase(0x91))));
  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (auto& tr : dev) {
      if (tr.position().x() < cx) {
        ASSERT_EQ(0x81, drives[tr.idx()].intensity.value());
        ASSERT_EQ(0x91, drives[tr.idx()].phase.value());
      } else {
        ASSERT_EQ(0, drives[tr.idx()].intensity.value());
        ASSERT_EQ(0, drives[tr.idx()].phase.value());
      }
    }
  }
}

TEST(Gain, GroupUnkownKey) {
  auto autd = create_controller();

  bool caught_err = false;
  try {
    autd.send(autd3::gain::Group([](const auto&) { return [](const auto&) -> std::optional<const char*> { return "null"; }; })
                  .set("uniform", autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90)))
                  .set("null", autd3::gain::Null()));
  } catch (autd3::AUTDException& e) {
    caught_err = true;
    ASSERT_STREQ("Unknown group key", e.what());
  }

  if (!caught_err) FAIL();
}

TEST(Gain, GroupCheckOnlyForEnabled) {
  auto autd = create_controller();
  autd[0].set_enable(false);

  std::vector check(2, false);
  autd.send(autd3::gain::Group([&check](const auto& dev) {
              return [&dev, &check](const auto&) -> std::optional<int> {
                check[dev.idx()] = true;
                return 0;
              };
            }).set(0, autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90))));

  ASSERT_FALSE(check[0]);
  ASSERT_TRUE(check[1]);

  {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}
