/*
 ==============================================================================
    Copyright (c) 2019-2022 Foleys Finest Audio - Daniel Walz
    All rights reserved.

    License for non-commercial projects:

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    License for commercial products:

    To sell commercial products containing this module, you are required to buy a
    License from https://foleysfinest.com/developer/pluginguimagic/

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================
 */

#include "foleys_MultiListPropertyComponent.h"

namespace foleys
{

MultiListPropertyComponent::MultiListPropertyComponent (const juce::Value& valueToControl, const juce::String& propertyName, const juce::StringArray &choicesToUse, const juce::String& separatorToUse, int preferredHeightToUse)
  : juce::PropertyComponent (propertyName, preferredHeightToUse),
    choices (choicesToUse),
    separator (separatorToUse)
{
    text.setEditable (true);
    text.getTextValue().referTo (valueToControl);
    text.onTextChange = [&]
    {
        auto strings = juce::StringArray::fromTokens (text.getText(), separator, "");
        strings.removeEmptyStrings (true);
        text.setText (strings.joinIntoString (separator), juce::sendNotificationAsync);
    };

    addAndMakeVisible (text);
    addAndMakeVisible (select);

    select.onClick = [&]
    {
        auto strings = juce::StringArray::fromTokens (text.getText(), separator, "");
        strings.removeEmptyStrings(true);

        juce::Component::SafePointer<juce::Label> textEdit (&text);

        juce::PopupMenu popup;
        for (const auto& name : choices)
        {
            auto newArray = strings;
            auto adding = !newArray.contains(name);

            if (adding)
                newArray.add(name);
            else
                newArray.removeString(name);

            auto newText = newArray.joinIntoString(separator);

            popup.addItem(name, true, !adding, [textEdit, newText]
            {
                if (textEdit)
                    textEdit->setText(newText, juce::sendNotification);
            });

        }
        popup.showMenuAsync (juce::PopupMenu::Options());
    };
}

void MultiListPropertyComponent::refresh()
{
}

void MultiListPropertyComponent::resized()
{
    auto area = getLookAndFeel().getPropertyComponentContentPosition (*this);

    select.setBounds (area.removeFromRight (getHeight()));
    text.setBounds (area);
}


} // namespace foleys
