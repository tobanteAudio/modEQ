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
// Audio
const float MAX_DB   = 24.0f;
const float MAX_GAIN = Decibels::decibelsToGain(tobanteAudio::MAX_DB);

// Gain
const float GAIN_MIN       = 0.0f;
const float GAIN_MAX       = 2.0f;
const float GAIN_STEP_SIZE = 0.01f;
const float GAIN_DEFAULT   = 1.0f;

// Filter
const float FILTER_GAIN_STEP_SIZE = 0.001f;
const float FILTER_FREQ_MIN       = 20.0f;
const float FILTER_FREQ_MAX       = 20'000.0f;
const float FILTER_FREQ_STEP_SIZE = 0.1f;
const float FILTER_Q_MIN          = 0.1f;
const float FILTER_Q_MAX          = 10.0f;
const float FILTER_Q_STEP_SIZE    = 0.1f;

// LFO
const float LFO_GAIN_MAX       = 1.0f;
const float LFO_FREQ_MIN       = 0.01f;
const float LFO_FREQ_MAX       = 10.0f;
const float LFO_FREQ_STEP_SIZE = 0.01f;
const float LFO_FREQ_SKEW      = 1.0f;
const float LFO_FREQ_DEFAULT   = 0.3f;

// UI
const int GLOBAL_REFRESH_RATE_HZ = 60;
const int HANDLE_CLICK_RADIUS    = 10;

}  // namespace tobanteAudio
