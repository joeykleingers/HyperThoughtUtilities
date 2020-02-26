/* ============================================================================
 * Copyright (c) 2020-2020 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "HTUtils.h"

#include <cstdlib>
#include <ctime>
#include <memory>

const char base64Table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

// -----------------------------------------------------------------------------
std::vector<int> getDecode64Vector()
{
  std::vector<int> decoded(256, -1); // 256 values, default -1
  for(size_t i = 0; i < 64; i++)
  {
    decoded[base64Table[i]] = i;
  }
  return decoded;
}

// -----------------------------------------------------------------------------
int getDecoded64Value(const unsigned char& character)
{
  static std::vector<int> decodedVec = getDecode64Vector();
  return decodedVec[character];
}

// -----------------------------------------------------------------------------
QString HTUtils::generateNonce(size_t length)
{
  // seed random generator with current time
  std::srand(std::time(nullptr));

  QString nonce;
  nonce.push_back(QString::number((std::rand() % 9) + 1)); // 1-9
  for(size_t i = 1; i < length; i++)
  {
    nonce.push_back(QString::number(std::rand() % 10)); // 0-9
  }
  return nonce;
}

// -----------------------------------------------------------------------------
QString HTUtils::encode64(const std::string& str)
{
  std::string encodedStr;

  // Hold remaining values between char values.
  // bitOverflow is the amount of bits remaining after encoding the current value
  int value = 0;
  int bitOverflow = -6; // First value read sets this to 2 (8-6=2)
  size_t length = str.length();
  // Iterate over all characters
  for(size_t i = 0; i < length; i++)
  {
    // Convert 8-bit characters to 6-bit encoding
    unsigned char character = str[i];
    value = (value << 8) + character; // Append character bits into value
    bitOverflow += 8;

    while(bitOverflow >= 0)
    {
      size_t code = (value >> bitOverflow) & 0x3F;
      char nextChar = base64Table[code];
      encodedStr.push_back(nextChar);
      bitOverflow -= 6;
    }
  }
  // Encode last char
  if(bitOverflow > -6)
  {
    // Extract 6 bits from the remaining bitOverflow amount.
    size_t code = ((value << 8) >> (bitOverflow + 8)) & 0x3F;
    char nextChar = base64Table[code];
    encodedStr.push_back(nextChar);
  }

  return QString::fromStdString(encodedStr);
}

// -----------------------------------------------------------------------------
QString HTUtils::decode64(const std::string& str)
{
  std::string decodedStr;

  // Hold remaining value between char values.
  // bitOverflow is the amount of bits remaining after decoding the current value
  int value = 0;
  int bitOverflow = -8;
  size_t length = str.length();
  // Iterate over all characters
  for(size_t i = 0; i < length; i++)
  {
    // Decode the next character to 6-bit value
    unsigned char character = str[i];
    if(getDecoded64Value(character) == -1)
    {
      // The end has been reached if the char does not decode.
      break;
    }
    value = (value << 6) + getDecoded64Value(character);
    bitOverflow += 6;

    // Add decoded char from 8-bit value
    if(bitOverflow >= 0)
    {
      char decodedChar = (value >> bitOverflow) & 0xFF;
      decodedStr.push_back(decodedChar);
      bitOverflow -= 8;
    }
  }

  return QString::fromStdString(decodedStr);
}

// -----------------------------------------------------------------------------
QString HTUtils::urlEncode(QString url)
{
  url.replace("%", "%25");
  // url.replace("\"", "%20");
  url.replace("!", "%21");
  url.replace("#", "%23");
  url.replace("$", "%24");
  //url.replace("%", "%25");
  url.replace("&", "%26");
  url.replace("'", "%27");
  url.replace("(", "%28");
  url.replace(")", "%29");
  url.replace("*", "%2A");
  url.replace("+", "%2B");
  url.replace(",", "%2C");
  url.replace("/", "%2F");
  url.replace(":", "%3A");
  url.replace(";", "%3B");
  url.replace("=", "%3D");
  url.replace("?", "%3F");
  url.replace("@", "40");
  url.replace("[", "%5B");
  url.replace("]", "%5D");
  url.replace(" ", "+");
  return url;
}

// -----------------------------------------------------------------------------
QString HTUtils::generateAuthUrl(const QString& authIpUrl, const QString& clientId)
{
  QString redirectUri = authIpUrl + "/access-code/";
  QString scope = "openid";
  QString nonce = generateNonce();
  QString responseType = "code";

  QString baseUrl = authIpUrl + "/openid/authorize/?";
  QString encoded = "client_id=" + clientId + "&redirect_uri=" + urlEncode(redirectUri);
  encoded += "&scope=" + scope + "&nonce=" + nonce + "&response_type=" + responseType;
  return baseUrl + encoded;
}

// -----------------------------------------------------------------------------
QMap<QString, QString> HTUtils::parseQueryParams(const QString& path)
{
  QMap<QString, QString> mappedValue;

  int initIndex = path.indexOf("/?") + 2;
  int endIndex = path.indexOf("{");
  int size = endIndex - initIndex;
  // First section
  QString parameterStr = path.mid(initIndex, size);
  QStringList parameters = parameterStr.split("&");
  for(const QString& param : parameters)
  {
    QStringList values = param.split("=");
    QString name = values[0];
    QString value = values[1];
    mappedValue[name] = value;
  }

  // JSonObj
  int objStart = endIndex + 1;
  int objEnd = path.indexOf("}") - 1;
  size = objEnd - objStart;
  QString objStr = path.mid(objStart, size);
  QStringList objValues = objStr.split(",");
  for(const QString& objValue : objValues)
  {
    QStringList items = objValue.split(":");
    QString name = items[0].trimmed().remove("\"");
    QString value = items[1].trimmed().remove("\"");
    mappedValue[name] = value;
  }

  return mappedValue;
}
