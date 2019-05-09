/* Copyright 2018-2019 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

namespace tobanteAudio
{
// UI
constexpr int GLOBAL_REFRESH_RATE_HZ = 60;
constexpr int clickRadius            = 10;

// Audio
constexpr float MAX_DB         = 24.0f;
constexpr float GAIN_MIN       = 0.0f;
constexpr float GAIN_MAX       = 2.0f;
constexpr float GAIN_STEP_SIZE = 0.01f;
constexpr float GAIN_DEFAULT   = 1.0f;

// LFO
constexpr float LFO_GAIN_MAX       = 1.0f;
constexpr float LFO_FREQ_MIN       = 0.01f;
constexpr float LFO_FREQ_MAX       = 10.0f;
constexpr float LFO_FREQ_STEP_SIZE = 0.01f;
constexpr float LFO_FREQ_SKEW      = 1.0f;
constexpr float LFO_FREQ_DEFAULT   = 0.3f;

}  // namespace tobanteAudio
