/*
 * Copyright (C) 2016 Huan N. Pham <huanbumdev@gmail.com>
 * All rights reserved.
 * All other uses outside TextCrypt Editor must be permitted
 * by Huan N. Pham.
 * The algorithm to encrypt texts provided by this library
 * should be altered before it is used personally due to
 * security purposes.
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include "TextCrypt.h"
#include "SHA512.h"
#define Encrypting true
#define Decrypting false
#define NumberOfTheEncryptable 256
#define KeyAdjustment 16
#define TAB 9
#define FirstCharacter 0
#define LastCharacter 255
using namespace std;
//Private Functions
void TextCrypt::SetParagraphs(){
	string Paragraph;
	while (!(Paragraphs_.size() >= 2 &&
			 Paragraphs_.at(Paragraphs_.size() - 1).empty() &&
			 Paragraphs_.at(Paragraphs_.size() - 2).empty())){
		fflush(stdin);
		getline(cin, Paragraph);
		Paragraphs_.push_back(Paragraph);
	}
	if (Paragraphs_.size() >= 2){
		Paragraphs_.pop_back();
		Paragraphs_.pop_back();
	}
}

void TextCrypt::SetParagraphsFromFile(string FileName){
	string Paragraph;
	InputFile_.open(FileName, ios::in);
	if (InputFile_.is_open()){
        cout << "Start reading file..." << endl;
		while (!(InputFile_.eof())){
			getline(InputFile_, Paragraph);
			Paragraphs_.push_back(Paragraph);
		}
		InputFile_.close();
		cout << "Finish reading file..." << endl;
	}
	else{
		cout << "Error: Unable to access file." << endl;
	}
}

void TextCrypt::SetPassword(){
	cout << "Password: ";
	do{
		fflush(stdin);
		getline(cin, Password_);
		if (Password_.empty()){
			cout << "Retry: ";
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
					if (*jt == TAB){
						*jt = ' ';
					}
				}
				if (!(it->empty())){
					while (it->back() == ' '){
						it->erase(it->end() - 1);
					}
					while (it->front() == ' '){
						it->erase(it->begin());
					}
					if (!(it->empty())){
						for (auto jt = it->begin(); jt != it->end(); jt++){
							if (*jt == ' ' && *(jt + 1) == ' '){
								while (*(jt + 1) == ' '){
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
			Words_.push_back({""});
		}
		else{
			vector<string> Word;
			int Beginning, Ending;
			Beginning = 0;
			for (unsigned int i = 0; i < it->size(); i++){
				if (it->at(i) == ' '){
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
    cout << "Start encrypting..." << endl;
	int GeneralKey = abs(int(Password_.back()) - int(Password_.front()));
	for (auto it = Words_.begin(); it != Words_.end(); it++){
		if (it->empty()){
			EncryptedParagraphs_.push_back("");
		}
		else{
			string EncryptedParagraph;
			for (auto jt = it->begin(); jt != it->end(); jt++){
				int Key = GeneralKey + jt->size();
				while (Key > NumberOfTheEncryptable){
					Key -= NumberOfTheEncryptable;
				}
				if (Key == NumberOfTheEncryptable){
					Key -= KeyAdjustment;
				}
				string EncryptedWord;
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
				EncryptedParagraph.push_back(' ');
			}
			EncryptedParagraph.pop_back();
			EncryptedParagraphs_.push_back(EncryptedParagraph);
		}
	}
	cout << "Finisih encrypting..." << endl;
}

void TextCrypt::SetDecryptedParagraphs(){
    cout << "Start decrypting..." << endl;
	int GeneralKey = abs(int(Password_.back()) - int(Password_.front()));
	for (auto it = Words_.begin(); it != Words_.end(); it++){
		if (it->empty()){
			DecryptedParagraphs_.push_back("");
		}
		else{
			string DecryptedParagraph;
			for (auto jt = it->begin(); jt != it->end(); jt++){
				int Key = GeneralKey + jt->size();
				while (Key > NumberOfTheEncryptable){
					Key -= NumberOfTheEncryptable;
				}
				if (Key == NumberOfTheEncryptable){
					Key -= KeyAdjustment;
				}
				string DecryptedWord;
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
				DecryptedParagraph.push_back(' ');
			}
			DecryptedParagraph.pop_back();
			DecryptedParagraphs_.push_back(DecryptedParagraph);
		}
	}
	cout << "Finish decrypting..." << endl;
}

void TextCrypt::SetHashedPassword(){
	if (StatusOfParagraphs_ == Encrypting){
		string SHA512HashedPassword = sha512(Password_);
		int Beginning = 0;
		for (unsigned int i = 0; i < 16; i++){
			HashedPassword_ += SHA512HashedPassword.substr(Beginning, 8);
			HashedPassword_.push_back(' ');
			Beginning += 8;
		}
		HashedPassword_.pop_back();
		for (auto it = HashedPassword_.begin(); it != HashedPassword_.end(); it++){
			const int Key = 8;
			if (*it != ' '){
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
		if (*it == ' '){
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
	if (HashedPassword_ == sha512(Password_)){
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
		cout << "Encrypted text:" << endl;
		for (auto it = EncryptedParagraphs_.begin(); it != EncryptedParagraphs_.end(); it++){
			cout << *it << endl;
		}
	}
	else{
		cout << "Decrypted text:" << endl;
		for (auto it = DecryptedParagraphs_.begin(); it != DecryptedParagraphs_.end(); it++){
			cout << *it << endl;
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
			cout << "Successfully encrypt that file..." << endl;
			OutputFile_.close();
		}
		else{
			cout << "Error: Unable to access file." << endl;
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
			cout << "Successfully decrypt that file..." << endl;
			OutputFile_.close();
		}
		else{
			cout << "Error: Unable to access file." << endl;
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
		cout << "Text:" << endl;
		SetParagraphs();
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			cout << "Error: Nothing needs encrypting." << endl;
		}
		else{
			SetPassword();
			SeparateWords();
			SetEncryptedParagraphs();
			SetHashedPassword();
			EncryptedParagraphs_.back() += (' ' + HashedPassword_);
			PrintResult();
		}
	}
	else{
		cout << "Encrypted text:" << endl;
		SetParagraphs();
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			cout << "Error: Nothing needs decrypting." << endl;
		}
		else{
			if (CheckHashedPassword() == false){
				cout << "Error: Lack of something." << endl;
			}
			else{
				SetHashedPassword();
				SetPassword();
				if (CheckPassword() == false){
					cout << "Error: Wrong password." << endl;
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
			cout << "Error: Nothing needs encrypting." << endl;
		}
		else{
			SetPassword();
			SeparateWords();
			SetEncryptedParagraphs();
			SetHashedPassword();
			EncryptedParagraphs_.back() += (' ' + HashedPassword_);
			PrintResultToFile(FileName);
		}
	}
	else{
		SetParagraphsFromFile(FileName);
		StandardizeParagraphs();
		if (CheckParagraphs() == false){
			cout << "Error: Nothing needs decrypting." << endl;
		}
		else{
			if (CheckHashedPassword() == false){
				cout << "Error: Lack of something." << endl;
			}
			else{
				SetHashedPassword();
				SetPassword();
				if (CheckPassword() == false){
					cout << "Error: Wrong password." << endl;
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

vector<string> TextCrypt::GetEncryptedParagraphs(){
	if (StatusOfParagraphs_ == Encrypting){
		return EncryptedParagraphs_;
	}
	else{
		return {"Error: Access Denied."};
	}
}

vector<string> TextCrypt::GetDecryptedParagraphs(){
	if (StatusOfParagraphs_ == Decrypting){
		return DecryptedParagraphs_;
	}
	else{
		return {"Error: Access Denied."};
	}
}
