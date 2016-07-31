/*
 * TextCrypt Copyright (C) 2016 Huan Pham
 * All rights reserved.
 *
 * The algorithm to encrypt texts provided by this library
 * should be altered before it is used personally due to
 * security purposes.
 *
 * This file is part of TextCrypt.
 *
 * TextCrypt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TextCrypt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TextCrypt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TEXTCRYPT_H_INCLUDED
#define TEXTCRYPT_H_INCLUDED
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class TextCrypt{
private:
    vector<wstring> Paragraphs_;
    vector<wstring> EncryptedParagraphs_;
    vector<wstring> DecryptedParagraphs_;
    vector<vector<wstring>> Words_;
    wstring Password_;
    wstring HashedPassword_;
    bool StatusOfParagraphs_;
    wifstream InputFile_;
    wofstream OutputFile_;
    void SetParagraphs();
    void SetParagraphsFromFile(string FileName);
    void SetPassword();
    void StandardizeParagraphs();
    string StandardizeFileName(string FileName);
    void SeparateWords();
    void SetEncryptedParagraphs();
    void SetDecryptedParagraphs();
    void SetHashedPassword();
    bool CheckPassword();
    bool CheckParagraphs();
    bool CheckHashedPassword();
    void PrintResult();
    void PrintResultToFile(string FileName);

public:
    TextCrypt();
    TextCrypt(bool StatusOfParagraphs);
    TextCrypt(bool StatusOfParagraphs, string FileName);
    ~TextCrypt();
    vector<wstring> GetEncryptedParagraphs();
    vector<wstring> GetDecryptedParagraphs();
};

#endif // TEXTCRYPT_H_INCLUDED
