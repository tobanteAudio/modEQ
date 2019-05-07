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

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
namespace tobanteAudio
{
class ModulationConnectItemView : public Component
{
public:
    //==============================================================================
    ModulationConnectItemView(int i);
    ~ModulationConnectItemView();

    //==============================================================================
    void resized() override;

    //==============================================================================
    Slider amount;
    Label target;

private:
    //==============================================================================
    int index;

    //==============================================================================
#if TOBANTEAUDIO_LIVE_MOCK
public:
    ModulationConnectItemView() : ModulationConnectItemView(1) { setSize(100, 100); }
#endif  // TOBANTEAUDIO_LIVE_MOCK

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationConnectItemView)
};

}  // namespace tobanteAudio