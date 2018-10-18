/* 
 * SPLPv1.c
 * The file is part of practical task for System programming course. 
 * This file contains definitions of the data structures and forward
 * declaration of handle_message() function
 */
#define _CRT_SECURE_NO_WARNINGS

enum ProtocolState
{
	INIT = 1,
	CONNECTING,
	CONNECTED,
	WAITING_VER, 
	WAITING_DATA,
	WAITING_B64_DATA,
	DISCONNECTING
};

enum test_status 
{ 
    MESSAGE_INVALID, 
    MESSAGE_VALID 
};


enum Direction 
{ 
    A_TO_B, 
    B_TO_A 
};

typedef struct CommandInfo_
{
	char* command;
	const unsigned char length;
	enum ProtocolState next_state;
} CommandInfo;

struct Message /* message */
{
	enum Direction	direction;        
	char			*text_message;
};


extern enum test_status validate_message( struct Message* pMessage ); 

char IsValidB64(char* c);

