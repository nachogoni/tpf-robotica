#include <../../protocolo/src/protocol.h>

short reset;
short crcOK;

char buffer[MAX_BUFFER_SIZE];
int buffer_write;
int buffer_read;
int data_length;

// Comando parseado
struct command_t command;
// Respuesta
struct command_t response;

// Interrupcion del RS232
#INT_RDA
void RS232()
{
	// Un nuevo dato...
	buffer[buffer_write++] = getc();
	data_length++;
	if (buffer_write == MAX_BUFFER_SIZE)
		buffer_write = 0;
	return;
}

/* Envia los datos por el pto serial */
void initProtocol()
{
	// Variables de comunicacion
	buffer_write = 0;
	buffer_read = 0;
	data_length = 0;
	crcOK = false;
	
	// Interrupcion Rcv
	enable_interrupts(INT_RDA);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);
}

/* Envia los datos por el pto serial */
void send(struct command_t * cmd)
{
	int i, len;
	
	len = cmd->len - 4;
	putc(cmd->len);
	putc(cmd->to);
	putc(cmd->from);
	putc(cmd->cmd);
	
	for (i = 0; i < len; i++)
	{
		putc((cmd->data)[i]);
	}
	
	// Enviar el CRC
	putc(cmd->crc);
	
	return;	
}	

void runProtocol(struct command_t * cmd)
{
	// Analiza el buffer
	if (data_length >= MIN_LENGTH && buffer[buffer_read] <= data_length)
	{
		data_length -= buffer[buffer_read] + 1;
	
		cmd->len = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd->to = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd->from = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd->cmd = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		// Obtiene el campo DATA
		if ((buffer_read + cmd->len - MIN_LENGTH) > MAX_BUFFER_SIZE)
		{
			// DATA esta partido en el buffer ciclico
			memcpy(cmd->data, buffer + buffer_read, MAX_BUFFER_SIZE - buffer_read);
			memcpy(cmd->data + MAX_BUFFER_SIZE - buffer_read, buffer, cmd->len - MIN_LENGTH - MAX_BUFFER_SIZE + buffer_read);
		} else {
			// DATA esta continuo
			memcpy(cmd->data, buffer + buffer_read, cmd->len - MIN_LENGTH);
		}
	
		buffer_read += cmd->len - MIN_LENGTH;
		if (buffer_read >= MAX_BUFFER_SIZE)
			buffer_read -= MAX_BUFFER_SIZE;
	
		cmd->crc = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;

		// Soy el destinatario?
		if (cmd->to == THIS_CARD)
		{
			// Ejecuta el comando
			doCommand(cmd);
		} else // Es broadcast?
			if ((cmd->to & 0xF0) == 0xF0)
		{
			// Ejecuta el comando
			doCommand(cmd);

			if (crcOK == true)
			{
				// Envia la respuesta
				send(&response);
				// Envia nuevamente el comando recibido
				response = *cmd;
			}
		} else // Es broadcast para mi grupo? 
			if ((cmd->to & THIS_GROUP) == THIS_GROUP)
		{
			// Ejecuta el comando
			doCommand(cmd);	
#ifdef RESEND_GROUP_BROADCAST
			if (crcOK == true)
			{
				// Envia la respuesta
				send(&response);
				// Envia nuevamente el comando recibido
				response = *cmd;
			}
#endif
		} else {
			response = *cmd;
		}
	
		// Envia la respuesta...
		send(&response);
	}
	
	// Reset del micro
	if (reset == true)
	{
		delay_ms(10);
		reset_cpu();
	}	

}