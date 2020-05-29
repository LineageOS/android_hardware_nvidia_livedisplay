/*
 * Copyright (C) 2019 The LineageOS Project
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

#include "DisplayColorCalibration.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace nvidia {

bool DisplayColorCalibration::isSupported() {
    return mLiveDisplay->IsCMUSupported();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayColorCalibration follow.
Return<int32_t> DisplayColorCalibration::getMaxValue() {
    return 255;
}

Return<int32_t> DisplayColorCalibration::getMinValue() {
    return 0;
}

Return<void> DisplayColorCalibration::getCalibration(getCalibration_cb _hidl_cb) {
    std::vector<uint16_t> csc;
    std::vector<int32_t> rgb;

    csc = mLiveDisplay->GetCMU();
    rgb.push_back(csc.at(0));
    rgb.push_back(csc.at(4));
    rgb.push_back(csc.at(8));

    _hidl_cb(rgb);
    return Void();
}

Return<bool> DisplayColorCalibration::setCalibration(const hidl_vec<int32_t>& rgb) {
    std::vector<uint16_t> csc(9, 0);

    csc.at(0) = static_cast<uint16_t>(rgb[0]);
    csc.at(4) = static_cast<uint16_t>(rgb[1]);
    csc.at(8) = static_cast<uint16_t>(rgb[2]);

    return mLiveDisplay->SetCMU(csc);
}

}  // namespace nvidia
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
