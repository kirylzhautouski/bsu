/* 
 * SPLPv1.c
 * The file is part of practical task for System programming course. 
 * This file contains validation of SPLPv1 protocol. 
 */


//RESULT - 12301/12373 Kirill Zheltovski FAMCS group 12

/*
---------------------------------------------------------------------------------------------------------------------------
# |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
  |                 | nection approval from srv |                                       |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |                     
  |                 |                           |        -------------------------------+-----------+----------------------
  |                 |                           |          One of the following:        |     5     |                      
  |                 |                           |          - GET_DATA                   |           |                      
  |                 |                           |          - GET_FILE                   |           |                      
  |                 |                           |          - GET_COMMAND                |           |
  |                 |                           |        -------------------------------+-----------+----------------------
  |                 |                           |          GET_B64                      |     6     |                      
  |                 |                           |        ------------------------------------------------------------------
  |                 |                           |          DISCONNECT                   |     7     |                                 
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2                     
  |                 | server to provide version |          Where ver is an integer (>0) |           |                      
  |                 | information               |          value. Only a single space   |           |                      
  |                 |                           |          is allowed in the message    |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD data CMD                 |     3     | GET_DATA a GET_DATA 
  |                 | response from server      |                                       |           |                      
  |                 |                           |          CMD - command sent by the    |           |                      
  |                 |                           |           client in previous message  |           |                      
  |                 |                           |          data - string which contains |           |                      
  |                 |                           |           the following allowed cha-  |           |                      
  |                 |                           |           racters: small latin letter,|           |                     
  |                 |                           |           digits and '.'              |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: data                    |     3     | B64: SGVsbG8=                    
  |                 | response from server.     |          where data is a base64 string|           |                      
  |                 |                           |          only 1 space is allowed      |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |                      
  |                 | server to close the       |                                       |           |                      
  |                 | connection                |                                       |           |                      
---------------------------------------------------------------------------------------------------------------------------

IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

*/


#include "splpv1.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


/* FUNCTION:  validate_message
 * 
 * PURPOSE:  
 *    This function is called for each SPLPv1 message between client 
 *    and server
 * 
 * PARAMETERS:
 *    msg - pointer to a structure which stores information about 
 *    message
 * 
 * RETURN VALUE:
 *    MESSAGE_VALID if the message is correct 
 *    MESSAGE_INVALID if the message is incorrect or out of protocol 
 *    state
 */
const char data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
const char base64[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

char IsValidB64(char* c)
{
	char *first_c = c;
	while (base64[*c])
		++c;

	char check = 0;
	while ((check < 2) && (c[check] == '='))
		++check;

	if (c[check] == 0 && (first_c - c + 1) % 4)
		return 1;

	return 0;
}

CommandInfo connected_commands[] = {
	{ "GET_DATA", 9, WAITING_DATA },
	{ "GET_COMMAND", 12, WAITING_DATA },
	{ "GET_FILE", 9, WAITING_DATA },
	{ "GET_B64", 8, WAITING_B64_DATA },
	{ "DISCONNECT", 11, DISCONNECTING },
	{ "GET_VER", 8, WAITING_VER },
};

enum ProtocolState state = INIT;
unsigned char previous_command;

enum test_status validate_message( struct Message *msg )
{
	char *text_message = msg->text_message;

	if (msg->direction == A_TO_B)
	{
		if (state == CONNECTED)
		{
			for (unsigned char i = 0; i < 6; i++)
			{
				if (!memcmp(text_message, connected_commands[i].command, connected_commands[i].length))
				{
					state = connected_commands[i].next_state;
					previous_command = i;
					return MESSAGE_VALID;
				}
			}
		}

		if (state == INIT && !memcmp(text_message, "CONNECT", 8))
		{
			state = CONNECTING;
			return MESSAGE_VALID;
		}

		state = INIT;
		return MESSAGE_INVALID;
	}

	if (msg->direction == B_TO_A)
	{
		if (state == WAITING_DATA)
		{
			char* previous_command_text = connected_commands[previous_command].command;
			unsigned char len = connected_commands[previous_command].length - 1;

			if (!memcmp(previous_command_text, text_message, len))
			{
				text_message += len;
				if (*text_message == ' ')
				{
					char* c = ++text_message;

					while (data[*c])
						++c;

					if (*c == ' ')
					{
						if (++c && !memcmp(c, previous_command_text, len + 1))
						{
							state = CONNECTED;
							return MESSAGE_VALID;
						}
					}
				}
			}
		}

		if (state == WAITING_B64_DATA && !memcmp("B64: ", text_message, 5))
		{
			if (IsValidB64(text_message + 5))
			{
				state = CONNECTED;
				return MESSAGE_VALID;
			}
		}

		if (state == DISCONNECTING && !memcmp("DISCONNECT_OK", text_message, 14))
		{
			state = INIT;
			return MESSAGE_VALID;
		}

		if (state == CONNECTING && !memcmp("CONNECT_OK", text_message, 11))
		{
			state = CONNECTED;
			return MESSAGE_VALID;
		}	

		if (state == WAITING_VER && !memcmp(text_message, "VERSION ", 8))
		{
			char* version_digit = text_message + 8;

			while (*version_digit != '\0')
			{
				if (*version_digit < 48 || *version_digit  > 57)
				{
					state = INIT;
					return MESSAGE_INVALID;
				}

				version_digit++;
			}

			state = CONNECTED;
			return MESSAGE_VALID;

		}

		state = INIT;
		return MESSAGE_INVALID;
	}

	state = INIT;
	return MESSAGE_INVALID;
}
  