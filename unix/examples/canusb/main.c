/*
  Copyright 2006 Lawicel AB

  A simple example to show how to use the Lawicel CANUSB under Linux.

*/

#include <stdio.h>
#include <string.h>
#include "ftd2xx.h"

#define MAX( a, b )( ( (a) > (b) ) ? (a) : (b) )
#define MIN( a, b )( ( (a) < (b) ) ? (a) : (b) )
#define ABS( a )(( (int) (a) < 0 ) ? ((a) ^ 0xffffffff) + 1 : (a) )

#define CANUSB_STATE_NONE  0
#define CANUSB_STATE_MSG   1


// CAN Frame
typedef struct {
  unsigned long id;         // Message id
  unsigned long timestamp;  // timestamp in milliseconds
  unsigned char flags;      // [extended_id|1][RTR:1][reserver:6]
  unsigned char len;        // Frame size (0.8)
  unsigned char data[ 8 ];  // Databytes 0..7
} CANMsg;

#define SPEED   6  // Speed for interface - See CANUSB 'O' command

#define BUF_SIZE 0x1000

// Message flags
#define CANMSG_EXTENDED   0x80 // Extended CAN id
#define CANMSG_RTR        0x40 // Remote frame

// Prototypes
BOOL canusbToCanMsg( char * p, CANMsg *pMsg );
BOOL SendTestFrames( FT_HANDLE ftHandle );
BOOL GetTestFrames( FT_HANDLE ftHandle );
BOOL readFrame( FT_HANDLE ftHandle );
BOOL openChannel( FT_HANDLE ftHandle, int nSpeed );
BOOL closeChannel( FT_HANDLE ftHandle );
BOOL sendFrame( FT_HANDLE ftHandle, CANMsg *pmsg );
void getVersionInfo(FT_HANDLE ftHandle);
void getSerialNumber( FT_HANDLE ftHandle );

// Globals
char gbufferRx[ BUF_SIZE ];
int gnReceivedFrames;

///////////////////////////////////////////////////////////////////////////////////////
// main
//

int main(int argc, char *argv[])
{
  char cBufRead[BUF_SIZE];
  DWORD dwBytesRead;
  FT_STATUS ftStatus;
  FT_HANDLE ftHandle;
  int iport;
  int i;
  char c;
  
  if(argc > 1) {
    sscanf(argv[1], "%d", &iport);
  }
  else {
    iport = 0;
  }
  
  // Note!
  // The second version of open should work from version 0.4.9 it may be prefered
  // in many situations. On Fedora Core 4, kernal 2.6.15 it fails however.
  //ftStatus = FT_Open(iport, &ftHandle);
  ftStatus = FT_OpenEx( "LWO65RKA", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
  //ftStatus = FT_OpenEx( NULL, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle); // First found
  if(ftStatus != FT_OK) {
    /* 
       This can fail if the ftdi_sio driver is loaded
       use lsmod to check this and rmmod ftdi_sio to remove
       also rmmod usbserial
    */
    printf("FT_Open(%d) failed. rv=%d\n", iport, ftStatus);
    return 1;
  }

  FT_SetTimeouts(ftHandle, 3000, 3000 );       // 3 second read timeout
     
  while ( 1 ) {
    
    printf("\n\n\n");
    printf("=====================================================================\n");
    printf("               Lawicel AB -- CANUSB test application\n");
    printf("=====================================================================\n\n");
   
    printf("S - Send test frames.\n");
    printf("R - Read five frames.\n");
    printf("N - Get Serial number.\n");
    printf("V - Get Version Information.\n");
    printf("Q - Quit application.\n");
   
    while ( 0x0a == ( c = getchar() ));

    if ( 'q' == c || 'Q' == c ) break;
    else if ( 'v' == c || 'V' == c  ){
      getVersionInfo( ftHandle );
    }
    else if ( 'n' == c || 'N' == c  ){
      getSerialNumber( ftHandle );
    }
    else if ( 's' == c || 'S' == c  ){
      SendTestFrames( ftHandle );
    }
    else if ( 'r' == c || 'R' == c  ){
      GetTestFrames( ftHandle );
    }
  }
	
  printf("Bye,bye....\n");
  FT_Close(ftHandle);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// getVersionInfo(FT_HANDLE ftHandle)
//

void getVersionInfo(FT_HANDLE ftHandle)
{
  FT_STATUS status;
  char buf[80];
  char c;
  char *p;
  unsigned long nBytesWritten;
  unsigned long eventStatus;
  unsigned long nRxCnt;// Number of characters in receive queue
  unsigned long nTxCnt;// Number of characters in transmit queue

  memset( buf, 0, sizeof( buf ) );
  printf("Get version.\n");
  printf("============\n");

  FT_Purge( ftHandle, FT_PURGE_RX | FT_PURGE_TX );

  sprintf( buf, "V\r" );
  if ( FT_OK != ( status = FT_Write( ftHandle, buf, strlen( buf ), &nBytesWritten ) ) ) {
    printf("Error: Failed to write command. return code = %d\n", status );
    return;
  }
  
  // Check if there is something to receive
  while ( 1 ){

    if ( FT_OK == FT_GetStatus( ftHandle, &nRxCnt, &nTxCnt, &eventStatus ) ) {
      
      // If there are characters to receive
      if ( nRxCnt ) {
	
	if ( FT_OK != ( status = FT_Read( ftHandle, buf, nRxCnt, &nBytesWritten ) ) ) {
	  printf("Error: Failed to read data. return code = %d\n", status );
	  return;
	}
	
	p = buf;
	while ( *p ) {
	  if ( 0x0d == *p ) {
	    *p = 0;
	    break;
	  }
	  p++;
	}
	printf( "Version = %s\n", buf );
	break;
	
      }
      
    }
    else {
      printf("Error: Failed to get status. return code = %d\n", status );
      return;
    }

  }
  
}


///////////////////////////////////////////////////////////////////////////////////////
// getSerialNumber(FT_HANDLE ftHandle)
//

void getSerialNumber( FT_HANDLE ftHandle )
{
  FT_STATUS status;
  char buf[80];
  char c;
  char *p;
  unsigned long nBytesWritten;
  unsigned long eventStatus;
  unsigned long nRxCnt;// Number of characters in receive queue
  unsigned long nTxCnt;// Number of characters in transmit queue

  memset( buf, 0, sizeof( buf ) );
  printf("Get serial number.\n");
  printf("==================\n");

  FT_Purge( ftHandle, FT_PURGE_RX | FT_PURGE_TX );

  sprintf( buf, "N\r" );
  if ( FT_OK != ( status = FT_Write( ftHandle, buf, strlen( buf ), &nBytesWritten ) ) ) {
    printf("Error: Failed to write command. return code = %d\n", status );
    return;
  }
  
  // Check if there is something to receive
  while ( 1 ){

    if ( FT_OK == FT_GetStatus( ftHandle, &nRxCnt, &nTxCnt, &eventStatus ) ) {
      
      // If there are characters to receive
      if ( nRxCnt ) {
	
	if ( FT_OK != ( status = FT_Read( ftHandle, buf, nRxCnt, &nBytesWritten ) ) ) {
	  printf("Error: Failed to read data. return code = %d\n", status );
	  return;
	}
	
	p = buf;
	while ( *p ) {
	  if ( 0x0d == *p ) {
	    *p = 0;
	    break;
	  }
	  p++;
	}
      
	printf( "Serial = %s \n", buf  );
	break;
	
      }
      
    }
    else {
      printf("Error: Failed to get status. return code = %d\n", status );
      return;
    }

  }
  
}

///////////////////////////////////////////////////////////////////////////////////////
// sendTestFrames(FT_HANDLE ftHandle)
//

BOOL SendTestFrames( FT_HANDLE ftHandle )
{
  CANMsg msg;

  if ( !openChannel( ftHandle, 6 ) ) {
    printf("Failed to open channel\n");
    return FALSE;
  }

  // Send extended frame
  msg.flags = CANMSG_EXTENDED;
  msg.id = 0x123456;
  msg.len = 2;
  msg.data[0] = 1;
  msg.data[1] = 2;

  if ( !sendFrame( ftHandle, &msg ) ) {
    printf("Failed to send frame\n");
    return FALSE;
  }

  // Send standard frame
  msg.flags = 0;
  msg.id = 0x123;
  msg.len = 2;
  msg.data[0] = 1;
  msg.data[1] = 2;

  if ( !sendFrame( ftHandle, &msg ) ) {
    printf("Failed to send frame\n");
    return FALSE;
  }
  

  FT_Purge( ftHandle, FT_PURGE_TX );

  if ( !closeChannel( ftHandle ) ) {
    printf("Failed to close channel\n");
    return FALSE;
  }
  
  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// GetTestFrames
//

BOOL GetTestFrames( FT_HANDLE ftHandle )
{
  gnReceivedFrames = 0;

  if ( !openChannel( ftHandle, 6 ) ) {
    printf("Failed to open channel\n");
    return FALSE;
  }

  printf("Waiting to receive five frames (ctrl-c to abort)...\n");
  while ( gnReceivedFrames < 5 ) {
    readFrame( ftHandle );
  }

  if ( !closeChannel( ftHandle ) ) {
    printf("Failed to close channel\n");
    return FALSE;
  }

  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// openChannel
//

BOOL openChannel( FT_HANDLE ftHandle, int nSpeed )
{
  char buf[80];
  unsigned long size;
  unsigned long retLen;
  
  // Set baudrate
  FT_Purge( ftHandle, FT_PURGE_RX );

  sprintf( buf, "S%d\r", nSpeed );
  size = 3;
  if ( !( FT_OK == FT_Write( ftHandle, buf, size, &retLen ) ) ) { 
    printf("Write failed\n");
    return FALSE;
  }
      
  // Open device
  FT_Purge( ftHandle, FT_PURGE_RX );
  strcpy( buf, "O\r" );
  size = 2;
  if ( !( FT_OK == FT_Write( ftHandle, buf, size, &retLen ) ) ) {
    printf("Write failed\n");
    return FALSE;
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// closeChannel
//

BOOL closeChannel( FT_HANDLE ftHandle )
{
  char buf[80];
  unsigned long size;
  unsigned long retLen;

  // Close device
  FT_Purge( ftHandle, FT_PURGE_RX | FT_PURGE_TX );
  strcpy( buf, "C\r" );
  size = 2;
  if ( !( FT_OK == FT_Write( ftHandle, buf, size, &retLen ) ) ) { 
    return FALSE;
  }

  
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// sendFrame
//

BOOL sendFrame( FT_HANDLE ftHandle, CANMsg *pmsg )
{
  int i; 
  char txbuf[80];
  unsigned long size;
  unsigned long retLen;

  if ( pmsg->flags & CANMSG_EXTENDED ) {
    if ( pmsg->flags & CANMSG_RTR ) {
      sprintf( txbuf, "R%08.8lX%i", pmsg->id, pmsg->len );
      pmsg->len = 0; 
    }
    else {
      sprintf( txbuf, "T%08.8lX%i", pmsg->id, pmsg->len );
    }
  }
  else {
    if ( pmsg->flags & CANMSG_RTR ) {
      sprintf( txbuf, "r%03.3lX%i", pmsg->id, pmsg->len );
      pmsg->len = 0; // Just dlc no data for RTR
    }
    else {
      sprintf( txbuf, "t%03.3lX%i", pmsg->id, pmsg->len );
    }
  }

  if ( pmsg->len ) {
    char hex[5];
    
    for ( i= 0; i< pmsg->len; i++ ) {
      sprintf( hex, "%02.2X", pmsg->data[i] );
      strcat( txbuf, hex );
    }
  }

  // Add CR
  strcat( txbuf, "\r" );

  size = strlen( txbuf );

  // Transmit fram
  if ( !( FT_OK == FT_Write( ftHandle, txbuf, size, &retLen ) ) ) { 
    return FALSE;
  }
  

  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// canusbToCanMsg
//

BOOL canusbToCanMsg( char *p, CANMsg *pMsg )
{
  int val;
  int i;
  short data_offset;   // Offset to dlc byte
  char save;

  if ( 't' == *p ) {
    // Standard frame
    pMsg->flags = 0;
    data_offset = 5;
    pMsg->len = p[ 4 ] - '0';
    p[ 4 ] = 0;
    sscanf( p + 1, "%lx", &pMsg->id  );
  }
  else if ( 'r' == *p ) {
    // Standard remote  frame
    pMsg->len = p[ 4 ] - '0';
    pMsg->flags = CANMSG_RTR;
    //data_offset = 5 - 1;// To make timestamp work
    data_offset = 5;
    //save = p[ 4 ];
    p[ 4 ] = 0;
    sscanf( p + 1, "%lx", &pMsg->id  );
    //p[ 4 ] = save;
  }
  else if ( 'T' == *p ) {
    // Extended frame
    pMsg->flags = CANMSG_EXTENDED;
    data_offset = 10;
    pMsg->len = p[ 9 ] - '0';
    p[ 9 ] = 0;
    sscanf( p + 1, "%lx", &pMsg->id );
  }
  else if ( 'R' == *p ) {
    // Extended remote frame
    pMsg->flags = CANMSG_EXTENDED | CANMSG_RTR;
    //data_offset = 10 - 1;// To make timestamp work
    data_offset = 10;
    pMsg->len = p[ 9 ] - '0';
    //save = p[ 9 ];
    p[ 9 ] = 0;
    sscanf( p + 1, "%lx", &pMsg->id );
    //p[ 9 ] = save;
  }
  
  save = *(p + data_offset + 2 * pMsg->len );
  
  // Fill in data
  if ( !( pMsg->flags & CANMSG_RTR ) ) {
    for ( i= MIN( pMsg->len, 8); i > 0; i-- ) {
      *(p + data_offset + 2 * (i-1) + 2 )= 0;
      sscanf( p + data_offset + 2 * (i-1), "%x", &val );
      pMsg->data[ i - 1 ] = val;
    }
  }

  *(p + data_offset + 2 * pMsg->len ) = save;

  if ( !( pMsg->flags & CANMSG_RTR ) ) {
    // If timestamp is active - fetch it
    if ( 0x0d != *( p + data_offset + 2 * pMsg->len ) ) {
      p[ data_offset + 2 * ( pMsg->len ) + 4 ] = 0;
      sscanf( ( p + data_offset + 2 * ( pMsg->len ) ), "%x", &val );
      pMsg->timestamp = val;
    }
    else {
      pMsg->timestamp = 0;
    }
  }
  else {
    
    if ( 0x0d != *( p + data_offset ) ) {
      p[ data_offset + 4 ] = 0;
      sscanf( ( p + data_offset ), "%x", &val );
      pMsg->timestamp = val;
    }
    else {
      pMsg->timestamp = 0;
    }
  }

  return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
// readFrame
//

BOOL readFrame( FT_HANDLE ftHandle )
{
  CANMsg msg;
  int i,j;
  char buf[80];
  unsigned long nRxCnt;
  unsigned long nTxCnt;
  unsigned long eventStatus;
  unsigned long nRcvCnt;
  char c;
  
  static char msgReceiveBuf[80];
  static int cntMsgRcv = 0;
  static int state = CANUSB_STATE_NONE;
  
  // Check if there is something to receive
  if ( FT_OK == FT_GetStatus( ftHandle, &nRxCnt, &nTxCnt, &eventStatus ) ) {
    
    // If there are characters to receive
    if ( nRxCnt ) {

      // Must fit to buffer
      if ( nRxCnt > sizeof( gbufferRx ) ) {
	nRxCnt = sizeof( gbufferRx );
      }
      
      // Read data
      if ( ( FT_OK == FT_Read( ftHandle, gbufferRx, nRxCnt, &nRcvCnt ) ) 
	   && ( nRcvCnt > 0 ) ) {
	
	for ( i=0; i<nRcvCnt; i++ ) {

	  // Get character
	  c = gbufferRx[ i ];
	  
	  if ( CANUSB_STATE_NONE == state ) {
	    
	    if ( ('t' == c ) || 
		 ( 'T' == c ) || 
		 ('r' == c ) || 
		 ( 'R' == c ) ) {
	      state = CANUSB_STATE_MSG;
	      memset( msgReceiveBuf, 0, sizeof( msgReceiveBuf ) );
	      msgReceiveBuf[ 0 ] = c;
	      cntMsgRcv = 1;
	    }
	    
	  }
	  
	  else if ( CANUSB_STATE_MSG == state ) {
	    
	    msgReceiveBuf[ cntMsgRcv++ ] = c;
	    
	    if ( 0x0d == c ) {
	   
	      printf("Raw Msg = %s\n", msgReceiveBuf );
	      if ( !canusbToCanMsg( msgReceiveBuf, &msg ) ) {
		printf("Message conversion failed!\n");
		state = CANUSB_STATE_NONE;
		return FALSE;
	      }
	       
	      if ( msg.flags & CANMSG_EXTENDED  ) {
		printf("Extended ");
	      }
	      else {
		printf("Standard ");
	      }

	      printf("message received: id=%X len=%d timestamp=%X ", 
		     msg.id, 
		     msg.len, 
		     msg.timestamp ); 
	      
	      if ( msg.len ) {
		
		printf("data=");
		
		for ( j=0; j<msg.len; j++ ) {
		  printf("%02X ", msg.data[j]);
		}
	 
	      }

	      printf("\n");

	      gnReceivedFrames++;

	      state = CANUSB_STATE_NONE;
	      
	    } // full message

	  } // STATE_MSG

	} // for each char

      } // Read data

    } // characters to receive

  } // getstatus
    
  return TRUE;
}
