/*****************************
 https://leetcode.com/problems/strong-password-checker/

A password is considered strong if below conditions are all met:

It has at least 6 characters and at most 20 characters.
It must contain at least one lowercase letter, at least one uppercase letter, and at least one digit.
It must NOT contain three repeating characters in a row ("...aaa..." is weak, but "...aa...a..." is strong, assuming other conditions are met).
Write a function strongPasswordChecker(s), that takes a string s as input, and return the MINIMUM change required to make s a strong password. If s is already strong, return 0.

Insertion, deletion or replace of any one character are all considered as one change.
***********/

#include <algorithm>
#include <ftl/catch_or_ignore.h>
#include <iostream>
#include <string>

using namespace std;
namespace {

class Solution {
public:
    int strongPasswordChecker(const string& s)
    {
        bool hasLower = false, hasUpper = false, hasDigit = false;
        size_t len = 0; // current password length
        const size_t MINLEN = 6, MAXLEN = 20, MAXREPEATING = 3;
        for (char c : s) {
            if (c >= 'a' && c <= 'z')
                hasLower = true;
            else if (c >= 'A' && c <= 'Z')
                hasUpper = true;
            else if (c >= '0' && c <= '9')
                hasDigit = true;
            ++len;
        }
        size_t nrep = 0, res = 0;
        char prev = -1; // invalid char
        const auto& mixedChars = [&] { // return true if ti's already mixed chars
            if (!hasLower)
                hasLower = true;
            else if (!hasUpper)
                hasUpper = true;
            else if (!hasDigit)
                hasDigit = true;
            else {
                return true;
            }
            return false;
        };
        const auto& onChange
            = [&](char c) {
                  ++res;
                  if (len > MAXLEN) {
                      --len;
                      --nrep;
                      return;
                  }
                  mixedChars(); // try to make it mixed
                  if (len < MINLEN) { // insert before current pos
                      ++len;
                      prev = c; // reset
                      nrep = 1;
                  } else { // change current char
                      prev = -1; // reset
                      nrep = 0;
                  }
              };
        for (auto c : s) {
            if (c != prev) {
                nrep = 1;
                prev = c;
            } else {
                ++nrep;
                if (nrep == MAXREPEATING) {
                    onChange(c);
                }
            }
        }

        if (len < MINLEN) {
            for (; len != MINLEN; ++len, ++res)
                mixedChars();
        } else if (len > MAXLEN) {
            for (; len != MAXLEN; --len, ++res)
                ;
        }
        while (!mixedChars())
            ++res;
        return res;
    }
};

TEST_FUNC(StrongPasswordChecker_test)
{
    Solution sln;

    //    REQUIRE(1 == sln.strongPasswordChecker("aaa23"));
    //    REQUIRE(2 == sln.strongPasswordChecker("aaa111"));
    //    REQUIRE(2 == sln.strongPasswordChecker("ABABABABABABABABABAB1"));
    //    REQUIRE(7 == sln.strongPasswordChecker("aaaaaaaaaaaaaaaaaaaaa"));
    REQUIRE(3 == sln.strongPasswordChecker("aaaabbaaabbaaa123456A"));
}
}
