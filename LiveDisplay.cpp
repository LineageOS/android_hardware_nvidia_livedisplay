/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "LiveDisplay-nvidia"

#include <android-base/logging.h>
#include <android-base/file.h>
#include <android-base/strings.h>

#include <video/tegra_dc_ext.h>

#include "LiveDisplay.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace nvidia {

LiveDisplay::LiveDisplay() {
    uint32_t numdc;
    struct tegra_dc_ext_cmu cmu;

    // Look up number of display controllers
    FILE* dcctrl = std::fopen("/dev/tegra_dc_ctrl", "r");
    ioctl(fileno(dcctrl), TEGRA_DC_EXT_CONTROL_GET_NUM_OUTPUTS, &numdc);
    fclose(dcctrl);

    // Loop over the reported DCs.
    for (uint32_t i = 0; i < numdc; i++) {
        FILE* tegradc = std::fopen(("/dev/tegra_dc_" + std::to_string(i)).c_str(), "rw");
        if (tegradc) {
            memset(&cmu, 0, sizeof(struct tegra_dc_ext_cmu));
            ioctl(fileno(tegradc), TEGRA_DC_EXT_GET_CMU, &cmu);

            // Try to enable cmu if it's disabled
            if (!cmu.cmu_enable) {
                cmu.cmu_enable = 1;
                ioctl(fileno(tegradc), TEGRA_DC_EXT_SET_CMU, &cmu);
                ioctl(fileno(tegradc), TEGRA_DC_EXT_GET_CMU, &cmu);
            }
            fclose(tegradc);

            // Save reported info in map
            LOG(INFO) << "Found dc." << std::to_string(i) << " with cmu "
                      << (cmu.cmu_enable ? "enabled" : "disabled") << ".";
            mDisplayControllers[static_cast<uint8_t>(i)] = cmu.cmu_enable;
        }
    }
}

bool LiveDisplay::IsCMUSupported()
{
    for (const auto& dc : mDisplayControllers) {
        if (dc.second)
            return true;
    }
    return false;
}

std::vector<uint16_t> const LiveDisplay::GetCMU()
{
    struct tegra_dc_ext_cmu cmu;
    std::vector<uint16_t> csc;
    uint8_t dcnum = std::numeric_limits<uint8_t>::max();

    for (const auto& dc : mDisplayControllers) {
        if (dc.second) {
            dcnum = dc.first;
            break;
        }
    }
    if (dcnum == std::numeric_limits<uint8_t>::max())
        return csc;

    FILE* tegradc = std::fopen(("/dev/tegra_dc_" + std::to_string(dcnum)).c_str(), "r");
    ioctl(fileno(tegradc), TEGRA_DC_EXT_GET_CMU, &cmu);
    fclose(tegradc);

    for (uint8_t i = 0; i < sizeof(cmu.csc)/sizeof(cmu.csc[0]); i++)
        csc.push_back(cmu.csc[i]);

    return csc;
}

bool LiveDisplay::SetCMU(std::vector<uint16_t> const &csc)
{
    for (const auto& dc : mDisplayControllers) {
        if (!dc.second)
            continue;

        struct tegra_dc_ext_cmu cmu;
        FILE* tegradc = std::fopen(("/dev/tegra_dc_" + std::to_string(dc.first)).c_str(), "rw");

        ioctl(fileno(tegradc), TEGRA_DC_EXT_GET_CMU, &cmu);

        for (uint8_t i = 0; i < sizeof(cmu.csc)/sizeof(cmu.csc[0]); i++)
            cmu.csc[i] = csc.at(i);

        ioctl(fileno(tegradc), TEGRA_DC_EXT_SET_CMU, &cmu);
        ioctl(fileno(tegradc), TEGRA_DC_EXT_GET_CMU, &cmu);

        fclose(tegradc);

        for (uint8_t i = 0; i < sizeof(cmu.csc)/sizeof(cmu.csc[0]); i++) {
            if (cmu.csc[i] != csc.at(i))
                return false;
        }
    }

    return true;
}

}  // namespace nvidia
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
