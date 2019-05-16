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

// JUCE
#include "../JuceLibraryCode/JuceHeader.h"

// tobanteAudio
#include "../Source/utils/text_value_converter.h"

namespace tobanteAudio::tests
{
class TestTextValueConverters : public UnitTest
{
public:
    TestTextValueConverters() : UnitTest("Text Value Converters") {}
    void runTest() override
    {
        {
            tobanteAudio::ActiveTextConverter a_converter {};

            beginTest("ActiveTextConverter -> String to float");
            const float f_active   = a_converter("active");
            const float f_bypassed = a_converter("bypassed");
            expect(f_active == 1.0f);
            expect(f_bypassed == 0.0f);

            beginTest("ActiveTextConverter -> float to String");
            const String s_active   = a_converter(1.0f);
            const String s_bypassed = a_converter(0.0f);
            expect(s_active == "active");
            expect(s_bypassed == "bypassed");
        }

        {
            tobanteAudio::InvertPhaseTextConverter invert_converter {};

            beginTest("InvertPhaseTextConverter -> String to float");
            const float f_normal   = invert_converter("Normal");
            const float f_inverted = invert_converter("Inverted");
            expect(f_normal == 0.0f);
            expect(f_inverted == 1.0f);

            beginTest("InvertPhaseTextConverter -> float to String");
            const String s_normal   = invert_converter(0.0f);
            const String s_inverted = invert_converter(1.0f);
            expect(s_normal == "Normal");
            expect(s_inverted == "Inverted");
        }
    }
};
}  // namespace tobanteAudio::tests