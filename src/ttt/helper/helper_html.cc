/*
  Copyright (c) 2018-2019, Kay Stenschke
  All rights reserved.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of  nor the names of its contributors may be used to
     endorse or promote products derived from this software without specific
     prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include <string>
#include <cstring>
#include <utility>
#include "helper_html.h"
#include "vendor/entities/decode_html_entities_utf8.h"

namespace helper {

std::string Html::Encode(std::string str) {
  std::string subs[] = {";", "&", "\"", "'", "<", ">", ":", "=", "!"};
  std::string reps[] = {"&semi;", "&amp;", "&quot;", "&apos;", "&lt;", "&gt;", "&colon;", "&equals;", "&excl;"};

  int amount_entities = 9;
  size_t found;

  for (int j = 0; j < amount_entities; j++) {
    do {
      found = str.find(subs[j]);

      if (std::string::npos!=found)
        str.replace(found, subs[j].length(), reps[j]);
    } while (std::string::npos!=found);
  }

  // Encode extended ascii characters
  std::wstring wide_str = std::wstring(str.begin(), str.end());
  auto str_len = static_cast<int>(wide_str.size());
  for (int i = str_len; i >= 0; i--) {
    char ch = str[i];
    if (ch & 128) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"
      switch ((int) str[i - 1]) {
        case -49:
          switch ((int) str[i]) {
            case -123:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&upsilon;");
              break;
            case -124:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&tau;");
              break;
            case -125:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&sigma;");
              break;
            case -126:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&sigmaf;");
              break;
            case -127:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&rho;");
              break;
          }
          break;
        case -50:
          switch ((int) str[i]) {
            case -65:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&omicron;");
              break;
            case -66:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&xi;");
              break;
            case -67:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&nu;");
              break;
            case -68:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&mu;");
              break;
            case -69:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&lambda;");
              break;
            case -70:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&kappa;");
              break;
            case -71:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&iota;");
              break;
            case -72:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&theta;");
              break;
            case -73:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&eta;");
              break;
            case -74:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&zeta;");
              break;
            case -75:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&epsilon;");
              break;
            case -76:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&delta;");
              break;
            case -77:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&gamma;");
              break;
            case -78:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&beta;");
              break;
            case -79:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&alpha;");
              break;
            case -93:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Sigma;");
              break;
          }
          break;
        case -53:
          switch ((int) str[i]) {
            case -100:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&tilde;");
              break;
            case -122:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&circ;");
              break;
          }
          break;
        case -59:
          switch ((int) str[i]) {
            case -72:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Yuml;");
              break;
            case -95:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&scaron;");
              break;
            case -96:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Scaron;");
              break;
            case -109:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&oelig;");
              break;
            case -110:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&OElig;");
              break;
          }
          break;
        case -61:
          switch ((int) str[i]) {
            case -65:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&yuml;");
              break;
            case -66:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&thorn;");
              break;
            case -67:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&yacute;");
              break;
            case -68:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&uuml;");
              break;
            case -69:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ucirc;");
              break;
            case -70:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&uacute;");
              break;
            case -71:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ugrave;");
              break;
            case -72:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&oslash;");
              break;
            case -73:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&divide;");
              break;
            case -74:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ouml;");
              break;
            case -75:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&otilde;");
              break;
            case -76:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ocirc;");
              break;
            case -77:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&oacute;");
              break;
            case -78:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ograve;");
              break;
            case -79:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ntilde;");
              break;
            case -80:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&eth;");
              break;
            case -81:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&iuml;");
              break;
            case -82:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&icirc;");
              break;
            case -83:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&iacute;");
              break;
            case -84:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&igrave;");
              break;
            case -85:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&euml;");
              break;
            case -86:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ecirc;");
              break;
            case -87:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&eacute;");
              break;
            case -88:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&egrave;");
              break;
            case -89:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ccedil;");
              break;
            case -90:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&aelig;");
              break;
            case -91:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&aring;");
              break;
            case -92:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&auml;");
              break;
            case -93:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&atilde;");
              break;
            case -94:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&acirc;");
              break;
            case -95:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&aacute;");
              break;
            case -96:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&agrave;");
              break;
            case -97:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&szlig;");
              break;
            case -99:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Yacute;");
              break;
            case -100:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Uuml;");
              break;
            case -101:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ucirc;");
              break;
            case -102:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Uacute;");
              break;
            case -103:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ugrave;");
              break;
            case -104:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Oslash;");
              break;
            case -105:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&times;");
              break;
            case -106:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ouml;");
              break;
            case -107:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Otilde;");
              break;
            case -108:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ocirc;");
              break;
            case -109:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Oacute;");
              break;
            case -110:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ograve;");
              break;
            case -111:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ntilde;");
              break;
            case -112:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ETH;");
              break;
            case -113:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Iuml;");
              break;
            case -114:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Icirc;");
              break;
            case -115:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Iacute;");
              break;
            case -116:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Igrave;");
              break;
            case -117:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Euml;");
              break;
            case -118:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ecirc;");
              break;
            case -119:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Eacute;");
              break;
            case -120:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Egrave;");
              break;
            case -121:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Ccedil;");
              break;
            case -122:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Aelig;");
              break;
            case -123:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Aring;");
              break;
            case -124:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Auml;");
              break;
            case -125:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Atilde;");
              break;
            case -126:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Acirc;");
              break;
            case -127:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Acute;");
              break;
            case -128:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&Agrave;");
              break;
          }
          break;
        case -62:
          switch ((int) str[i]) {
            case -65:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&iquest;");
              break;
            case -66:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&frac34;");
              break;
            case -67:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&frac12;");
              break;
            case -68:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&frac14;");
              break;
            case -69:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&raquo;");
              break;
            case -74:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&para;");
              break;
            case -77:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&sup3;");
              break;
            case -78:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&sup2;");
              break;
            case -79:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&plusmn;");
              break;
            case -80:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&deg;");
              break;
            case -82:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&reg;");
              break;
            case -85:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&laquo;");
              break;
            case -86:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&ordf;");
              break;
            case -87:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&copy;");
              break;
            case -88:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&uml;");
              break;
            case -89:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&sect;");
              break;
            case -90:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&brvbar;");
              break;
            case -91:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&yen;");
              break;
            case -92:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&curren;");
              break;
            case -93:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&pound;");
              break;
            case -94:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&cent;");
              break;
            case -95:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, L"&iexcl;");
              break;
          }
          break;
        case -126:
          switch ((int) str[i]) {
            case -84:
              if ((int) str[i - 2]==-30)
                wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, i - 2, i + 1, L"&euro;");
              break;
          }
          break;
        case -129:
          switch ((int) str[i]) {
            case -71:
              switch ((int) str[i - 2]) {
                case -29:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, i - 2, i + 1, L"&rsaquo;");
                  break;
                case -30:wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, i - 2, i + 1, L"&lsaquo;");
                  break;
              }
              break;
            case -80:
              if ((int) str[i - 2]==-30)
                wide_str = ReplaceWideCharByEntity(wide_str, str_len, i, i - 2, i + 1, L"&permil;");
              break;
          }
          break;
      }
#pragma clang diagnostic pop
    }
//}
  }
  str = std::string(wide_str.begin(), wide_str.end());

  return str;
}

std::wstring Html::ReplaceWideCharByEntity(
    std::wstring wstr, int str_len,
    int offset, int offset_left, int offset_right,
    std::wstring replacement) {
  return offset < str_len
         ? wstr.substr(0, offset_left) + replacement + wstr.substr(offset_right, std::string::npos)
         : wstr.substr(0, offset_left) + replacement;
}

std::wstring Html::ReplaceWideCharByEntity(std::wstring wstr, int str_len, int offset, std::wstring replacement) {
  return ReplaceWideCharByEntity(std::move(wstr), str_len, offset, offset - 1, offset + 1, std::move(replacement));
}

std::string Html::Decode(std::string str) {
  auto *dest = new char[str.length() + 1];
  strcpy(dest, str.c_str());
  decode_html_entities_utf8(dest, nullptr);

  return std::string(dest);
}

} // namespace helper
