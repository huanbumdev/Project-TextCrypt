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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "TextCrypt.h"
#include "SHA512.h"
#define Encrypting true
#define Decrypting false
#define NumberOfTheEncryptable 65536
#define KeyAdjustment 16
#define TAB 9
#define FirstCharacter 0
#define LastCharacter 65535
using namespace std;
//Private Functions
void TextCrypt::SetParagraphs(){
	wstring Paragraph;
	while (!(Paragraphs_.size() >= 2 &&
			 Paragraphs_.at(Paragraphs_.size() - 1).empty() &&
			 Paragraphs_.at(Paragraphs_.size() - 2).empty())){
		fflush(stdin);
		getline(wcin, Paragraph);
		Paragraphs_.push_back(Paragraph);
	}
	if (Paragraphs_.size() >= 2){
		Paragraphs_.pop_back();
		Paragraphs_.pop_back();
	}
}

void TextCrypt::SetParagraphsFromFile(string FileName){
	wstring Paragraph;
	InputFile_.open(FileName, ios::in);
	if (InputFile_.is_open()){
        wcout << "Start reading file..." << endl;
		while (!(InputFile_.eof())){
			getline(InputFile_, Paragraph);
			Paragraphs_.push_back(Paragraph);
		}
		InputFile_.close();
		wcout << "Finish reading file..." << endl;
	}
	else{
		wcout << "Error: Unable to access file." << endl;
	}
}

void TextCrypt::SetPassword(){
	wcout << "Password: ";
	do{
		fflush(stdin);
		getline(wcin, Password_);
		if (Password_.empty()){
			wcout << "Retry: ";
		}
	} while (Password_.empty());
}

void TextCrypt::StandardizeParagraphs(){
	if (!(Paragraphs_.empty())){
		for (auto it = Paragraphs_.begin(); it != Paragraphs_.end(); it++){
			if (it == Paragraphs_.begin() && it->empty()){
				Paragraphs_.erase(it);
			}
			if (it == Paragraphs_.end() - 1 && it->empty()){
				Paragraphs_.erase(it);
			}
			if (!(Paragraphs_.empty())){
				for (auto jt = it->begin(); jt != it->end(); jt++){
					if (*jt == L'\t'){
						*jt = L' ';
					}
				}
				if (!(it->empty())){
					while (it->back() == L' '){
						it->erase(it->end() - 1);
					}
					while (it->front() == L' '){
						it->erase(it->begin());
					}
					if (!(it->empty())){
						for (auto jt = it->begin(); jt != it->end(); jt++){
							if (*jt == L' ' && *(jt + 1) == L' '){
								while (*(jt + 1) == L' '){
									it->erase(jt);
								}
							}
						}
					}
				}
			}
		}
	}
}

string TextCrypt::StandardizeFileName(string FileName){
    while (FileName.front() == '"'){
        FileName.erase(FileName.begin());
    }
    while (FileName.back() == '"'){
        FileName.erase(FileName.end() - 1);
    }
    return FileName;
}

void TextCrypt::SeparateWords(){
	for (auto it = Paragraphs_.begin(); it != Paragraphs_.end(); it++){
		if (it->empty()){
			Words_.push_back({L""});
		}
		else{
			vector<wstring> Word;
			int Beginning, Ending;
			Beginning = 0;
			for (unsigned int i = 0; i < it->size(); i++){
				if (it->at(i) == L' '){
					Ending = i;
					Word.push_back(it->substr(Beginning, Ending - Beginning));
					Beginning = Ending + 1;
				}
			}
			Word.push_back(it->substr(Beginning, it->size() - Beginning));
			Words_.push_back(Word);
		}
	}
}

void TextCrypt::SetEncryptedParagraphs(){
    wcout << "Start encrypting..." << endl;
	int GeneralKey = abs(int(Password_.back()) - int(Password_.front()));
	for (auto it = Words_.begin(); it != Words_.end(); it++){
		if (it->empty()){
			EncryptedParagraphs_.push_back(L"");
		}
		else{
			wstring EncryptedParagraph;
			for (auto jt = it->begin(); jt != it->end(); jt++){
				int Key = GeneralKey + jt->size();
				while (Key > NumberOfTheEncryptable){
					Key -= NumberOfTheEncryptable;
				}
				if (Key == NumberOfTheEncryptable){
					Key -= KeyAdjustment;
				}
				wstring EncryptedWord;
				for (auto kt = jt->begin(); kt != jt->end(); kt++){
					if ((*kt + Key) > LastCharacter){
						*kt += (Key - NumberOfTheEncryptable);
					}
					else{
						*kt += Key;
					}
					EncryptedWord.push_back(*kt);
				}
				EncryptedParagraph += EncryptedWord;
				EncryptedParagraph.push_back(L' ');
			}
			EncryptedParagraph.pop_back();
			EncryptedParagraphs_.push_back(EncryptedParagraph);
		}
	}
	wcout << "Finish encrypting..." << endl;
}

void TextCrypt::SetDecryptedParagraphs(){
    wcout << "Start decrypting..." << endl;
	int GeneralKey = abs(int(Password_.back()) - int(Password_.front()));
	for (auto it = Words_.begin(); it != Words_.end(); it++){
		if (it->empty()){
			DecryptedParagraphs_.push_back(L"");
		}
		else{
			wstring DecryptedParagraph;
			for (auto jt = it->begin(); jt != it->end(); jt++){
				int Key = GeneralKey + jt->size();
				while (Key > NumberOfTheEncryptable){
					Key -= NumberOfTheEncryptable;
				}
				if (Key == NumberOfTheEncryptable){
					Key -= KeyAdjustment;
				}
				wstring DecryptedWord;
				for (auto kt = jt->begin(); kt != jt->end(); kt++){
					if ((*kt - Key) < FirstCharacter){
						*kt -= (Key - NumberOfTheEncryptable);
					}
					else{
						*kt -= Key;
					}
					DecryptedWord.push_back(*kt);
				}
				DecryptedParagraph += DecryptedWord;
				DecryptedParagraph.push_back(L' ');
			}
			DecryptedParagraph.pop_back();
			DecryptedParagraphs_.push_back(DecryptedParagraph);
		}
	}
	wcout << "Finish decrypting..." << endl;
}

void TextCrypt::SetHashedPassword(){
	if (StatusOfParagraphs_ == Encrypting){
        string TemporaryPassword;
        string TemporarySHA512HashedPassword;
        wstring SHA512HashedPassword;
        TemporaryPassword.assign(Password_.begin(), Password_.end());
        TemporarySHA512HashedPassword = sha512(TemporaryPassword);
        SHA512HashedPassword.assign(TemporarySHA512HashedPassword.begin(), TemporarySHA512HashedPassword.end());
		int Beginning = 0;
		for (unsigned int i = 0; i < 16; i++){
			HashedPassword_ += SHA512HashedPassword.substr(Beginning, 8);
			HashedPassword_.push_back(L' ');
			Beginning += 8;
		}
		HashedPassword_.pop_back();
		for (auto it = HashedPassword_.begin(); it != HashedPassword_.end(); it++){
			const int Key = 8;
			if (*it != L' '){
				if ((*it + Key) > LastCharacter){
					*it += (Key - NumberOfTheEncryptable);
				}
				else{
					*it += Key;
				}
			}
		}
	}
	else{
		HashedPassword_ = Paragraphs_.back().substr(Paragraphs_.back().size() - 144, 144);
		Paragraphs_.back().erase(Paragraphs_.back().size() - 144, 144);
	}
}

bool TextCrypt::CheckPassword(){
	for (auto it = HashedPassword_.begin(); it != HashedPassword_.end(); it++){
		if (*it == L' '){
			HashedPassword_.erase(it);
		}
	}
	for (auto it = HashedPassword_.begin(); it != HashedPassword_.end(); it++){
		const int Key = 8;
		if ((*it - Key) < FirstCharacter){
			*it -= (Key - NumberOfTheEncryptable);
		}
		else{
			*it -= Key;
		}
	}
    string TemporaryHashedPassword;
    string TemporaryPassword;
    TemporaryHashedPassword.assign(HashedPassword_.begin(), HashedPassword_.end());
    TemporaryPassword.assign(Password_.begin(), Password_.end());
	if (TemporaryHashedPassword == sha512(TemporaryPassword)){
		return true;
	}
	else{
		return false;
	}
}

bool TextCrypt::CheckParagraphs(){
	for (auto it = Paragraphs_.begin(); it != Paragraphs_.end(); it++){
		if (!(it->empty())){
			return true;
		}
	}
	return false;
}

bool TextCrypt::CheckHashedPassword(){
	if (Paragraphs_.back().size() < 144){
		return false;
	}
	else{
		return true;
	}
}

void TextCrypt::PrintResult(){
	if (StatusOfParagraphs_ == Encrypting){
		wcout << "Encrypted text:" << endl;
		for (auto it = EncryptedParagraphs_.begin(); it != EncryptedParagraphs_.end(); it++){
			wcout << *it << endl;
		}
	}
	else{
		wcout << "Decrypted text:" << endl;
		for (auto it = DecryptedParagraphs_.begin(); it != DecryptedParagraphs_.end(); it++){
			wcout << *it << endl;
		}
	}
}

void TextCrypt::PrintResultToFile(string FileName){
	OutputFile_.open(FileName, ios::out | ios::trunc);
	if (StatusOfParagraphs_ == Encrypting){
		if (OutputFile_.is_open()){
			for (auto it = EncryptedParagraphs_.begin(); it != EncryptedParagraphs_.end(); it++){
				if (it == EncryptedParagraphs_.end() - 1){
					OutputFile_ << *it;
				}
				else{
					OutputFile_ << *it << endl;
				}
			}
			wcout << "Successfully encrypt that file..." << endl;
			OutputFile_.close();
		}
		else{
			wcout << "Error: Unable to access file." << endl;
		}
	}
	else{
		if (OutputFile_.is_open()){
			for (auto it = DecryptedParagraphs_.begin(); it != DecryptedParagraphs_.end(); it++){
				if (it == DecryptedParagraphs_.end() - 1){
					OutputFile_ << *it;
				}
				else{
					OutputFile_ << *it << endl;
				}
			}
			wcout << "Successfully decrypt that file..." << endl;
			OutputFile_.close();
		}
		else{
			wcout << "Error: Unable to access file." << endl;
		}
	}
}
//Public Functions
TextCrypt::TextCrypt(){
	Paragraphs_.clear();
	EncryptedParagraphs_.clear();
	DecryptedParagraphs_.clear();
	Words_.clear();
	Password_.clear();
	HashedPassword_.clear();
}

TextCrypt::TextCrypt(bool StatusOfParagraphs){
	StatusOfParagraphs_ = StatusOfParagraphs;
	if (StatusOfParagraphs_ == Encrypting){
		wcout << "Text:" << endl;
		SetParagraphs();
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			wcout << "Error: Nothing needs encrypting." << endl;
		}
		else{
			SetPassword();
			SeparateWords();
			SetEncryptedParagraphs();
			SetHashedPassword();
			EncryptedParagraphs_.back() += (L' ' + HashedPassword_);
			PrintResult();
		}
	}
	else{
		wcout << "Encrypted text:" << endl;
		SetParagraphs();
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			wcout << "Error: Nothing needs decrypting." << endl;
		}
		else{
			if (CheckHashedPassword() == false){
				wcout << "Error: Lack of something." << endl;
			}
			else{
				SetHashedPassword();
				SetPassword();
				if (CheckPassword() == false){
					wcout << "Error: Wrong password." << endl;
				}
				else{
					SeparateWords();
					SetDecryptedParagraphs();
					PrintResult();
				}
			}
		}
	}
}

TextCrypt::TextCrypt(bool StatusOfParagraphs, string FileName){
	StatusOfParagraphs_ = StatusOfParagraphs;
	FileName = StandardizeFileName(FileName);
	if (StatusOfParagraphs_ == Encrypting){
		SetParagraphsFromFile(FileName);
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			wcout << "Error: Nothing needs encrypting." << endl;
		}
		else{
			SetPassword();
			SeparateWords();
			SetEncryptedParagraphs();
			SetHashedPassword();
			EncryptedParagraphs_.back() += (L' ' + HashedPassword_);
			PrintResultToFile(FileName);
		}
	}
	else{
		SetParagraphsFromFile(FileName);
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			wcout << "Error: Nothing needs decrypting." << endl;
		}
		else{
			if (CheckHashedPassword() == false){
				wcout << "Error: Lack of something." << endl;
			}
			else{
				SetHashedPassword();
				SetPassword();
				if (CheckPassword() == false){
					wcout << "Error: Wrong password." << endl;
				}
				else{
					SeparateWords();
					SetDecryptedParagraphs();
					PrintResultToFile(FileName);
				}
			}
		}
	}
}

TextCrypt::~TextCrypt(){
	Paragraphs_.clear();
	EncryptedParagraphs_.clear();
	DecryptedParagraphs_.clear();
	Words_.clear();
	Password_.clear();
	HashedPassword_.clear();
}

vector<wstring> TextCrypt::GetEncryptedParagraphs(){
	if (StatusOfParagraphs_ == Encrypting){
		return EncryptedParagraphs_;
	}
	else{
		return {L"Error: Access Denied."};
	}
}

vector<wstring> TextCrypt::GetDecryptedParagraphs(){
	if (StatusOfParagraphs_ == Decrypting){
		return DecryptedParagraphs_;
	}
	else{
		return {L"Error: Access Denied."};
	}
}
