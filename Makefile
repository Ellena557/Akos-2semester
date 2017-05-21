all: serverch clientch

serverch: server.c client_util_f_message_format_struct.c server_util_f_user_side.c server_til_f_message_reciving_struct.c server_util_mem.c
	gcc -o runChatServer server.c client_util_f_message_format_struct.c server_util_f_user_side.c server_til_f_message_reciving_struct.c server_util_mem.c Common.c -I -d -pthread

clientch:
	gcc -o startChatClient client.c client_util_f_message_format_struct.c -pthread
