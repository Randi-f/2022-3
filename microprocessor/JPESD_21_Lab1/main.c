#include <stdint.h>

/*__asm void my_strcpy(const char *src, char *dst)
{
loop
    LDRB  r2, [r0]  // Load byte into r2 from memory pointed to by r0 (src pointer)
    ADDS  r0, #1    // Increment src pointer
    STRB  r2, [r1]  // Store byte in r2 into memory pointed to by r1 (dst pointer)
    ADDS  r1, #1    // Increment dst pointer
    CMP   r2, #0    // Was the byte 0?
    BNE   loop      // If not, repeat the loop
    BX    lr        // Else return from subroutine
}

__asm void my_capitalise(char *str)
{
cap_loop
    LDRB  r1, [r0]    // Load byte into r1 from memory pointed to by r0 (str pointer)
    CMP   r1, #'a'    // compare it with the 'a' character
    BLO   cap_skip    // If byte is lower or same, then skip this byte
    
    CMP   r1, #'z'    // Compare it with the 'z' character
    BHI   cap_skip    // If it is higher, then skip this byte
    
    SUBS  r1,#32      // Else subtract out difference to capitalize it
    STRB  r1, [r0]    // Store the capitalized byte back in memory
    
cap_skip
    ADDS  r0, r0, #1  // Increment str pointer
    CMP   r1, #0      // Was the byte 0?
    BNE   cap_loop    // If not, repeat the loop
    BX    lr          // Else return from subroutine
}
*/

uint32_t roman_num_val(char c){
 switch (c) {
 case 'I': return 1; 
 case 'V': return 5; 
 case 'X': return 10; 
 case 'L': return 50; 
 case 'C': return 100; 
 case 'D': return 500; 
 case 'M': return 1000;
 }
 return 0;
}
__asm  uint32_t roman_to_dec(char *s){
	extern roman_num_val
		//your assembly starts here
	PUSH {r4-r6,lr} //use stack
	MOV r2,r0 //r2 stores r0
	MOVS r6,#0x00 //r6 is sum, starting at 0
	MOVS r3,#0x00 //r3 is i, starting at 0
	
	WHILE_BEGIN
		LDRB r0, [r2,r3]//r0=s[i]
		CMP r0,NOP //s[i]!='\0'
		BEQ WHILE_END 
		BL.W roman_num_val
		ADDS r1,r3,#1 //r1=s[i+1]
		MOV r4,r0// r4 is decimal return of s[i]
		LDRB r0,[r2,r1]//r0 = s[i+1]
		BL.W roman_num_val
		MOV r5,r0//
		IFBEGIN
			CMP r4,r5
			BCS ELSEPART
		IFPART
			SUBS r6,r6,r4
			B IFEND
		ELSEPART
			ADDS r6,r6,r4
		IFEND
		ADDS r3,r3,#1//i++
		B WHILE_BEGIN
		WHILEEND
		MOV r0,r6
		POP {r4-r6,pc}
	
		
}
/*
uint32_t roman_to_dec(char *s){
 uint32_t i, x, y;
 uint32_t sum = 0;
 for (i = 0; s[i] != '\0'; i++){
 x = roman_num_val(s[i]); // value of the current char
 y = roman_num_val(s[i + 1]); // value of the next char
 if(x<y){
	 sum = sum-x;
 }
 else{
	 sum = sum+x;
 }
 //sum = (x < y) ? sum - x : sum + x;
 }
 return sum;
 }
*/
 
int main(void){
  //create a string of roman numerals
	char ch = '\0';
 char s[] = "XXIII";
 // run the assembly function
 uint32_t v = roman_to_dec(s);
 //printf("%d", (int)v);
 while (1);
}

/*int main(void)
{
    //const char a[] = "Hello qmul!";
    //char b[20];

    //my_strcpy(a, b);
    //my_capitalise(b);
    
    //while (1);
}
*/
//*****updated by Matthew Tang in March 2021***************
//*****ARM University Program Copyright © ARM Ltd 2016*****
