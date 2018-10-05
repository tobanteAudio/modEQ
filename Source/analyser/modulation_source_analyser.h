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
namespace TA
{
template <typename Type> class ModulationSourceAnalyser : public Thread
{
public:
    ModulationSourceAnalyser() : Thread("ModulationSource-Analyser"), abstractFifo(48000) {}

    ~ModulationSourceAnalyser() override = default;

    void addAudioData(const AudioBuffer<Type>& buffer, int startChannel, int numChannels)
    {
        if (abstractFifo.getFreeSpace() < buffer.getNumSamples()) return;

        int start1, block1, start2, block2;
        abstractFifo.prepareToWrite(buffer.getNumSamples(), start1, block1, start2, block2);
        audioFifo.copyFrom(0, start1, buffer.getReadPointer(startChannel), block1);
        if (block2 > 0)
            audioFifo.copyFrom(0, start2, buffer.getReadPointer(startChannel, block1), block2);

        for (int channel = startChannel + 1; channel < startChannel + numChannels; ++channel)
        {
            if (block1 > 0) audioFifo.addFrom(0, start1, buffer.getReadPointer(channel), block1);
            if (block2 > 0)
                audioFifo.addFrom(0, start2, buffer.getReadPointer(channel, block1), block2);
        }
        abstractFifo.finishedWrite(block1 + block2);
        waitForData.signal();
    }

    void setupAnalyser(int audioFifoSize, Type sampleRateToUse)
    {
        sampleRate = sampleRateToUse;
        audioFifo.setSize(1, audioFifoSize);
        abstractFifo.setTotalSize(audioFifoSize);
        analyserBuffer.setSize(1, int(sampleRate));

        audioFifo.clear();
        analyserBuffer.clear();

        startThread(5);
    }

    void run() override
    {
        while (!threadShouldExit())
        {
            if (abstractFifo.getNumReady() >= int(sampleRate / 30))
            {
                analyserBuffer.clear();

                int start1, block1, start2, block2;
                abstractFifo.prepareToRead(int(sampleRate / 30), start1, block1, start2, block2);

                if (block1 > 0)
                    analyserBuffer.copyFrom(0, 0, audioFifo.getReadPointer(0, start1), block1);
                if (block2 > 0)
                    analyserBuffer.copyFrom(0, block1, audioFifo.getReadPointer(0, start2), block2);

                abstractFifo.finishedRead(block1 + block2);

                ScopedLock lockedForWriting(pathCreationLock);

                newDataAvailable = true;
            }

            if (abstractFifo.getNumReady() < int(sampleRate / 30)) waitForData.wait(100);
        }
    }

    void createPath(Path& p, const Rectangle<float> bounds, float /*minFreq*/)
    {
        p.clear();
        ScopedLock lockedForReading(pathCreationLock);

        const auto* reader    = analyserBuffer.getReadPointer(0);
        const auto numSamples = analyserBuffer.getNumSamples();

        const auto factor = bounds.getWidth() / 10.0f;

        p.startNewSubPath(bounds.getX(), ampToY(reader[1], bounds));

        for (int i = 0; i < numSamples; ++i)
        {
            p.lineTo(bounds.getX() + factor * indexToX(i, numSamples, bounds),
                     ampToY(reader[i], bounds));
            i += 25;
        }
    }

    bool checkForNewData()
    {
        auto available   = newDataAvailable;
        newDataAvailable = false;
        return available;
    }

private:
    inline float indexToX(int index, int numSamples, const Rectangle<float> bounds) const
    {
        return jmap(static_cast<float>(index), 0.0f, static_cast<float>(numSamples), bounds.getX(),
                    bounds.getRight());
    }

    inline float ampToY(float bin, const Rectangle<float> bounds) const
    {
        return jmap(bin, -1.f, 1.0f, bounds.getBottom(), bounds.getY());
    }

    Type sampleRate{};

    AbstractFifo abstractFifo;
    AudioBuffer<Type> audioFifo;
    AudioBuffer<float> analyserBuffer;
    bool newDataAvailable = false;

    WaitableEvent waitForData;
    CriticalSection pathCreationLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationSourceAnalyser)
};

}  // namespace TA