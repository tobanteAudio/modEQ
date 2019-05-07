/* Copyright 2018 Tobias Hienzsch
 *
 * modEQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * modEQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with modEQ.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
namespace TA
{
class BandView : public Component
{
public:
    //==============================================================================
    BandView(int);
    ~BandView() override;

    //==============================================================================
    void resized() override;

    //==============================================================================
    GroupComponent frame;
    ComboBox type;
    Slider frequency;
    Slider quality;
    Slider gain;
    TextButton solo;
    TextButton activate;

private:
    //==============================================================================
    int index;

#if TOBANTEAUDIO_LIVE_MOCK
public:
    BandView() : BandView(1) { setSize(100, 100); }
#endif  // TOBANTEAUDIO_LIVE_MOCK

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandView)
};

}  // namespace TA