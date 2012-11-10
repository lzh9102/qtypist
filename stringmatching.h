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

#ifndef STRINGMATCHING_H
#define STRINGMATCHING_H

#include <QString>

/**
 * @brief Calculate the similarity of the two strings.
 *
 * Similarity of two strings is defined to be LCS(s1,s2)/MAX(LEN(s1),LEN(s2))
 *
 * @param s1 the first input string
 * @param s2 the second input string
 * @return similarity between 0.0 (no common element) ~ 1.0 (perfect match)
 */
double similarity(const QString& s1, const QString& s2);

#endif // STRINGMATCHING_H
