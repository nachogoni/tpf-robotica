#include <../../protocolo/src/protocol.h>

short reset;
short crcOK;
short sendResponse;

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
	disable_interrupts(INT_RDA);
	// Un nuevo dato...
	buffer[buffer_write++] = getc();
	data_length++;
	if (buffer_write == MAX_BUFFER_SIZE)
		buffer_write -= MAX_BUFFER_SIZE;
	enable_interrupts(INT_RDA);
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
	if (buffer[buffer_read] < data_length)
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

		sendResponse = true;

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
			if (((cmd->to & 0x0F) == 0x0F) &&
			 	((cmd->to & 0xF0) == THIS_GROUP))
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
	
		// Envia la respuesta?
		if (sendResponse == true)
		{
			send(&response);
		}	

	}
	
	// Reset del micro
	if (reset == true)
	{
		reset_cpu();
	}	

}

int generate_8bit_crc(char* data, int length, int pattern)
{
	// TODO: reemplazar por el crc?
	
	int crc_byte, i;

	crc_byte = data[0];
			
	for (i = 1; i < length; i++)
		crc_byte ^= data[i];
	
   /*int   *current_data;
   int   crc_byte;
   int byte_counter;
   int   bit_counter;

   current_data = data;
   crc_byte = *current_data++;

   for(byte_counter=0; byte_counter < (length-1); byte_counter++)
   {
      for(bit_counter=0; bit_counter < 8; bit_counter++)
      {
         if(!bit_test(crc_byte,7))
         {
            crc_byte <<= 1;
            bit_test(*current_data, 7 - bit_counter) ?
               bit_set(crc_byte,0) : bit_clear(crc_byte,0);
            continue;
         }
         crc_byte <<= 1;
         bit_test(*current_data, 7 - bit_counter) ?
            bit_set(crc_byte,0) : bit_clear(crc_byte,0);
         crc_byte ^= pattern;
      }
      current_data++;
   }
   for(bit_counter=0; bit_counter < 8; bit_counter++)
   {
      if(!bit_test(crc_byte,7))
      {
         crc_byte <<= 1;
         continue;
      }
      crc_byte <<= 1;
      crc_byte ^= pattern;
   }*/
   return crc_byte;
}