#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int STDIN_FD = 0;
const int BUFFER_SIZE = 100;
const int CHUNK_SIZE = 8;

void asciiFromDecimal(char* s, int decimalValue) {
  int reducedDecimalValue = decimalValue % 128;
  if (isascii(reducedDecimalValue) && reducedDecimalValue > 32) {
    sprintf(s, "%c  ", (char) reducedDecimalValue);
  } else {
    switch (reducedDecimalValue) {
      case 0: sprintf(s, "NUL"); return;
      case 1: sprintf(s, "SOH"); return;
      case 2: sprintf(s, "STX"); return;
      case 3: sprintf(s, "ETX"); return;
      case 4: sprintf(s, "EOT"); return;
      case 5: sprintf(s, "ENQ"); return;
      case 6: sprintf(s, "ACK"); return;
      case 7: sprintf(s, "BEL"); return;
      case 8: sprintf(s, "BS "); return;
      case 9: sprintf(s, "HT "); return;
      case 10: sprintf(s, "NL "); return;
      case 11: sprintf(s, "VT "); return;
      case 12: sprintf(s, "NP "); return;
      case 13: sprintf(s, "CR "); return;
      case 14: sprintf(s, "SO "); return;
      case 15: sprintf(s, "SI "); return;
      case 16: sprintf(s, "DLE"); return;
      case 17: sprintf(s, "DC1"); return;
      case 18: sprintf(s, "DC2"); return;
      case 19: sprintf(s, "DC3"); return;
      case 20: sprintf(s, "DC4"); return;
      case 21: sprintf(s, "NAK"); return;
      case 22: sprintf(s, "SYN"); return;
      case 23: sprintf(s, "ETB"); return;
      case 24: sprintf(s, "CAN"); return;
      case 25: sprintf(s, "EM "); return;
      case 26: sprintf(s, "SUB"); return;
      case 27: sprintf(s, "ESC"); return;
      case 28: sprintf(s, "FS "); return;
      case 29: sprintf(s, "GS "); return;
      case 30: sprintf(s, "RS "); return;
      case 31: sprintf(s, "US "); return;
      case 32: sprintf(s, "SP "); return;

      default: sprintf(s, "NONE"); return;
    }
  }
}

void readIntoBuffer(int argc, char const *argv[], int BUFFER_SIZE, char * buffer) {
  read(STDIN_FD, buffer, BUFFER_SIZE);
  for (int i = 0; i < BUFFER_SIZE; i++) {
    if (buffer[i] == '\n' || buffer[i] == ' ') {
      buffer[i] = '\0';
      break;
    }
  }
  int fd = open(buffer, O_RDONLY);
  printf("%s", buffer);
  printf("%d", fd);
  if (fd > 0) {
    read(fd, buffer, BUFFER_SIZE);
  }
}

int main(int argc, char const *argv[]) {
  char buffer[BUFFER_SIZE];
  readIntoBuffer(argc, argv, BUFFER_SIZE, buffer);

  printf("Original ASCII    Decimal  Parity\n");
  printf("-------- -------- -------- --------\n");

  int counter = 0;
  while (counter < BUFFER_SIZE) {
    if (buffer[counter] == '\0' || buffer[counter] == '\n') {
      return 0;
    }

    while (buffer[counter] == ' ' || buffer[counter] == '-') {
      counter++;
    }

    char originalValue[CHUNK_SIZE + 1];
    originalValue[CHUNK_SIZE] = '\0';
    int decimalValue = 0;
    int amtOfOnes = 0;
    int i;
    for (i = counter; i < counter + CHUNK_SIZE; i++) {
      char b = buffer[i];

      if (!(b == '1' || b == '0' || b == ' ' || b == '\0' || b == '\n')) {
        printf("Illegal input %c", b); //put middle finger ascii here
        return 0;
      }

      if (b == '1') {
        amtOfOnes++;
        originalValue[i - counter] = '1';
        decimalValue = (decimalValue << 1) + 1;
      } else if (b == '0') {
        originalValue[i - counter] = '0';
        decimalValue = (decimalValue << 1);
      } else {
        for (int j = i; j < counter + CHUNK_SIZE; j++) {
          originalValue[j - counter] = '0';
          decimalValue = (decimalValue << 1);
        }
        break;
      }
    }
    counter = i;

    char asciiValue[4];
    asciiFromDecimal(asciiValue, decimalValue);
    char* parityValue = amtOfOnes % 2 == 1 ? "ODD " : "EVEN";
    printf("%s %s      %d       %s\n", originalValue, asciiValue, decimalValue, parityValue);
  }

  return 0;
}