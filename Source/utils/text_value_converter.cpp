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

#include "text_value_converter.h"
#include "../processor/equalizer_processor.h"

namespace tobanteAudio
{
String FilterTypeTextConverter::operator()(float const value) const
{
    using EP = tobanteAudio::EqualizerProcessor;
    return EP::getFilterTypeName(static_cast<EP::FilterType>(static_cast<int>(value)));
}

float FilterTypeTextConverter::operator()(const String& text) const
{
    using EP = tobanteAudio::EqualizerProcessor;

    for (int i = 0; i < EP::LastFilterID; ++i)
    {
        if (text == EP::getFilterTypeName(static_cast<EP::FilterType>(i)))
        {
            return static_cast<float>(i);
        }
    }
    return static_cast<float>(EP::NoFilter);
}
}  // namespace tobanteAudio