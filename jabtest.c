/*
 * Copyright (C) 2015 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include "jabtest.h"

typedef struct {
    const char *file;
    const char *function;
    unsigned int line;
    const char *expression;
} FailureData;

static FailureData *g_failures = NULL;
static unsigned int g_numFailures = 0;

static unsigned int g_testsPassed = 0;
static unsigned int g_testsFailed = 0;

static void
addFailure(const char *file, const char *function, unsigned int line,
           const char *expression)
{
    FailureData *data;

    g_failures = (FailureData *)realloc(g_failures, sizeof(FailureData) * (g_numFailures + 1));
    if(!g_failures) {
        fprintf(stderr, "Unable to allocate memory for failure data\n");
        exit(1);
    }

    data = &g_failures[g_numFailures++];
    data->file = file;
    data->function = function;
    data->line = line;
    data->expression = expression;
}

TestResult
assert(const char *file, const char *function, unsigned int line,
       const char *expression, TestResult result)
{
    if(!result)
        addFailure(file, function, line, expression);

    return result;
}

void
run_test(TestFunction function)
{
    if(function()) {
        ++g_testsPassed;
        printf("\033[1;32m.\033[0m");
    } else {
        ++g_testsFailed;
        printf("\033[1;31m!\033[0m");
    }

    fflush(stdout);
}

void
print_test_summary()
{
    /* print information about failures */
    if(g_numFailures != 0) {
        unsigned int i;

        printf("\n\n\033[1;31mFailures:\033[0m\n");

        for(i = 0; i < g_numFailures; ++i) {
            FailureData *data = &g_failures[i];
            printf(" - %s (%s, line %u)\n   Assertion '%s' failed\n", data->function, data->file, data->line, data->expression);
        }
    }

    /* print number of tests passed/failed */
    printf("\n");
    printf("%d test%s passed\n", g_testsPassed, (g_testsPassed == 1 ? "" : "s"));
    printf("%d test%s failed\n", g_testsFailed, (g_testsFailed == 1 ? "" : "s"));

    /* clean up for subsequent test runs */
    if(g_failures)
        free(g_failures);
    g_numFailures = 0;
    g_testsPassed = 0;
    g_testsFailed = 0;
}
