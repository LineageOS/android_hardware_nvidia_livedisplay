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

#include "ReadingEnhancement.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace nvidia {

static const std::vector<uint16_t> csc_grayscale = {54, 182, 18, 54, 182, 18, 54, 182, 18};
static std::vector<uint16_t> csc_restore;

// Methods from ::vendor::lineage::livedisplay::V2_0::IReadingEnhancement follow.
bool ReadingEnhancement::isSupported() {
    return mLiveDisplay->IsCMUSupported();
}

Return<bool> ReadingEnhancement::isEnabled() {
    std::vector<uint16_t> csc_current = mLiveDisplay->GetCMU();

    return csc_current == csc_grayscale;
}

Return<bool> ReadingEnhancement::setEnabled(bool enabled) {
    bool ret = true;

    if (enabled) {
        csc_restore = mLiveDisplay->GetCMU();
        ret = mLiveDisplay->SetCMU(csc_grayscale);
    } else {
        ret = mLiveDisplay->SetCMU(csc_restore);
        csc_restore.clear();
    }

    return ret;
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace nvidia
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
