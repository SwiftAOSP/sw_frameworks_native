/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once

#include <stdint.h>
#include <ui/GraphicTypes.h>
#include <optional>
#include <vector>

#include <system/graphics.h>
#include <utils/Flattenable.h>

namespace android {

struct HdrMetadata : public LightFlattenable<HdrMetadata> {
    enum Type : uint32_t {
        SMPTE2086 = 1 << 0,
        CTA861_3 = 1 << 1,
        HDR10PLUS = 1 << 2,
    };

    uint32_t validTypes{0};

    android_smpte2086_metadata smpte2086{};
    android_cta861_3_metadata cta8613{};
    std::vector<uint8_t> hdr10plus{};

    // LightFlattenable
    bool isFixedSize() const { return false; }
    size_t getFlattenedSize() const;
    status_t flatten(void* buffer, size_t size) const;
    status_t unflatten(void const* buffer, size_t size);

    std::optional<ui::Smpte2086> getSmpte2086() const {
        if (validTypes & Type::SMPTE2086) {
            return ui::translate(smpte2086);
        }
        return {};
    }

    std::optional<ui::Cta861_3> getCta8613() const {
        if (validTypes & Type::CTA861_3) {
            return ui::translate(cta8613);
        }
        return {};
    }

    std::optional<std::vector<uint8_t>> getHdr10Plus() const {
        if (validTypes & Type::HDR10PLUS) {
            return hdr10plus;
        }
        return {};
    }

    bool operator==(const HdrMetadata& rhs) const;
    bool operator!=(const HdrMetadata& rhs) const { return !(*this == rhs); }
};

} // namespace android
