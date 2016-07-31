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
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "TextCrypt.h"
#include "SHA512.h"
#define Encrypting true
#define Decrypting false
using namespace std;

int main(){
    int Option;
    string FileName;
    cout << "Option 1: Encrypt text." << endl;
    cout << "Option 2: Decrypt text." << endl;
    cout << "Instruction: Hit Enter twice to stop typing." << endl;
    cout << "Option 3: Encrypt file." << endl;
    cout << "Option 4: Decrypt file." << endl;
    cout << "Your option: ";
    do{
        fflush(stdin);
        cin >> Option;
        if (Option != 1 && Option != 2 && Option != 3 && Option != 4){
            cout << "Retry: ";
        }
    } while (Option != 1 && Option != 2 && Option != 3 && Option != 4);
    if (Option == 1){
        TextCrypt Text(Encrypting);
    }
    else if (Option == 2){
        TextCrypt Text(Decrypting);
    }
    else if (Option == 3){
        cout << "File: ";
        do {
            fflush(stdin);
            getline(cin, FileName);
            if (FileName.empty()){
                cout << "Retry: ";
            }
        } while (FileName.empty());
        TextCrypt(Encrypting, FileName);
    }
    else if (Option == 4){
        cout << "File: ";
        do {
            fflush(stdin);
            getline(cin, FileName);
            if (FileName.empty()){
                cout << "Retry: ";
            }
        } while (FileName.empty());
        TextCrypt(Decrypting, FileName);
    }
    system("pause");
    return 0;
}
