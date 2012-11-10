/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: http://code.google.com/p/qtypist
 */

#include "stringmatching.h"
#include <QString>
#include <algorithm>
#include <vector>

namespace
{

/**
 * @brief Calculate the length of the longest common subsequence of s1 and s2.
 *
 * This function only needs to calculate the length of the LCS, so the table
 * for dynamic programming only occupies 2*s2.size() integers. The time complexity
 * of this function is s1.size() * s2.size().
 * @param s1 the first input string
 * @param s2 the second input string
 * @return the length of the LCS
 */
int lcs_len(const QString& s1, const QString& s2)
{
    std::vector<int> v1, v2; // table for dynamic programming
    std::vector<int> *pv1 = &v1, *pv2 = &v2;

    const int size1 = s1.size(), size2 = s2.size();

    pv1->resize(size2 + 1, 0);
    pv2->resize(size2 + 1, 0);

    for (int i=0; i<size1; i++) {
        for (int j=0; j<size2; j++) {
            if (s1.at(i) == s2.at(j))
                pv2->at(j + 1) = pv1->at(j) + 1;
            else
                pv2->at(j + 1) = std::max(pv2->at(j), pv1->at(j + 1));
        }
        std::swap(pv1, pv2);
    }

    return pv1->at(size2); // the last result is swapped to pv1
}

} // anonymous namespace

double similarity(const QString& s1, const QString& s2)
{
    if (s1.isEmpty() && s2.isEmpty())
        return 0.0;
    else
        return ((double)lcs_len(s1, s2)) / std::max(s1.length(), s2.length());
}
