#pragma once

// Message Id shared with the client and the server
enum ChatMessage
{
	ChatUserJoin,
	ChatUserQuit,
	ChatText,
	ChatBigMessage
};