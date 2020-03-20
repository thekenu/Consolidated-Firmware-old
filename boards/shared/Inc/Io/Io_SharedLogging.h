/**
 * @brief Library for logging information
 */
#ifndef SHARED_LOGGING_H
#define SHARED_LOGGING_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "lib/SEGGER_RTT/SEGGER_RTT.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Printf-style function to print data to the terminal using Segger RTT
 * @param  sFormat Pointer to format string, followed by the arguments for
 *         conversion
 */
void Io_SharedLogging_Printf(const char *sFormat, ...);

#endif // SHARED_LOGGING_H
