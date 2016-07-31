/*
 * Copyright (C) 2016 Huan N. Pham <huanbumdev@gmail.com>
 * All rights reserved.
 * All other uses outside TextCrypt Editor must be permitted
 * by Huan N. Pham.
 * The algorithm to encrypt texts provided by this library
 * should be altered before it is used personally due to
 * security purposes.
 */

#ifndef TEXTCRYPT_H_INCLUDED
#define TEXTCRYPT_H_INCLUDED
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class TextCrypt{
private:
    vector<string> Paragraphs_;
    vector<string> EncryptedParagraphs_;
    vector<string> DecryptedParagraphs_;
    vector<vector<string>> Words_;
    string Password_;
    string HashedPassword_;
    bool StatusOfParagraphs_;
    ifstream InputFile_;
    ofstream OutputFile_;
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
    vector<string> GetEncryptedParagraphs();
    vector<string> GetDecryptedParagraphs();
};

#endif // TEXTCRYPT_H_INCLUDED
