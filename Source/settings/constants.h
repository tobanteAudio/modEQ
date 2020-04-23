/* Copyright 2018-2020 Tobias Hienzsch
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

/**
 * @brief namespace for all tobanteAudio classes, functions & constants.
 */
namespace tobanteAudio
{
// Audio
constexpr auto MAX_DB      = 24.0f;
inline auto const MAX_GAIN = Decibels::decibelsToGain(tobanteAudio::MAX_DB);

// Gain
constexpr auto GAIN_MIN       = 0.0f;
constexpr auto GAIN_MAX       = 2.0f;
constexpr auto GAIN_STEP_SIZE = 0.01f;
constexpr auto GAIN_DEFAULT   = 1.0f;

// Filter
constexpr auto FILTER_GAIN_STEP_SIZE = 0.001f;
constexpr auto FILTER_FREQ_MIN       = 20.0f;
constexpr auto FILTER_FREQ_MAX       = 20'000.0f;
constexpr auto FILTER_FREQ_STEP_SIZE = 0.1f;
constexpr auto FILTER_Q_MIN          = 0.1f;
constexpr auto FILTER_Q_MAX          = 10.0f;
constexpr auto FILTER_Q_STEP_SIZE    = 0.1f;

// LFO
constexpr auto LFO_GAIN_MAX       = 1.0f;
constexpr auto LFO_FREQ_MIN       = 0.01f;
constexpr auto LFO_FREQ_MAX       = 10.0f;
constexpr auto LFO_FREQ_STEP_SIZE = 0.01f;
constexpr auto LFO_FREQ_SKEW      = 1.0f;
constexpr auto LFO_FREQ_DEFAULT   = 0.3f;

// UI
/**
 * @brief Global frames per second.
 */
const int GLOBAL_REFRESH_RATE_HZ = 60;
/**
 * @brief Click radius for band handles in analyser plot.
 */
const int HANDLE_CLICK_RADIUS = 10;

// COLORS
const auto ORANGE = Colour(255, 87, 34);
}  // namespace tobanteAudio
