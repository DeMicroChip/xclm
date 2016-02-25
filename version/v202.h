/*
 * The MIT License (MIT)
 *
 * Copyright (c)2015-2016, does not matter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef V202_H
#define V202_H

#include <string>
#include "abstractversion.h"

/**
 * this file contains xclm version specific digest and lookup table
 */

/**
 * @brief digest of xclm
 * Microchip XC License Manager: xclm (v2.02)
 * Build Date: Jun  4 2015
 * Copyright (c) 2011-2013,  Microchip Technology Inc. All rights reserved.
 */
class XclmV202 : public AbstractVersion
{
    public:
        explicit XclmV202() = default;
        virtual ~XclmV202() = default;

        virtual const std::string& hiddenHashTable() const override {
            return XC8_KEY_V202;
        }
        virtual const std::string& xclmDigest() const override {
            return XCLM_V202_DIGEST;
        }

        virtual const std::string&  name() const override {
            return VERSION;
        }

    private:
        const std::string XC8_KEY_V202 = "12020569031595942853997381615114499907649862923404988817922715553"
                                         "41838205786313090186455873609335258146199157795260719418491995998"
                                         "67328321377639683720790016145394178294936006671919157552224249424"
                                         "39615639096641032911590957809655146512799184051057152559880154371"
                                         "09781102039827532566787603522336984941661811057014715778639499737"
                                         "52378527793703095602570185318279000307654710756304884332086971157"
                                         "37423807934450316076253177145354444118311781822497185263570918244"
                                         "89987962035083357561720226033937858703281312678079900541773486911"
                                         "52537065623705744096622171290262732073236149224291304052855537234"
                                         "10330775777980642420243048828152100091460265382206962715520208227"
                                         "43350010152948011986901176259516763669981718355752348807037195557"
                                         "42347294083595208861666202572853755813079282586487282173705566196"
                                         "89895266201877681062920081779233813587682842641243243148028217367"
                                         "45067206935076268953043459393750329663637757506247332399234828831"
                                         "0773390527680200757984356";

        const std::string XCLM_V202_DIGEST = "789ccd7875aaa6d56eda14c59e34cd7806e1face";

        const std::string VERSION = "xclm (v2.02)";
};

#endif // V202_H
