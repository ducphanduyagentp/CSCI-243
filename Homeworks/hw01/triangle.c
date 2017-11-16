/// 
/// File: triangle.c 
/// 
/// A program to print out multiple "pyramid" triangles of the form:
/// <pre>
///   *
///  ***
/// *****
/// </pre>
/// 
/// @author ddp3945 : Duc Phan
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>

/**
 *  Function: drawTriangle
 *
 *  Description: Draw a 'pyramid' triangle whose base width is 'size'.
 *  Note: If size is even, the function makes a 'size + 1' pyramid triangle. 
 * 
 *  @param size  the width of the base of the triangle to draw
 */
void drawTriangle(int size) {
    size += 1 - (size & 1);
    int i, j;
    for (i = 1; i <= size; i+= 2) {
	printf("  ");
        for (j = 0; j < (size - i) / 2; j++) {
            printf(" ");
        }
        for (j = 0; j < i; j++) {
            printf("*");
        }
        puts("");
    }
    puts("");
}

/**
 *  Function: main 
 *
 *  Description: draws pyramid triangles of size 1, 5, and '6'.
 *
 *  @returns errorCode  error Code; EXIT_SUCCESS if no error
 */
int main() {
	puts("");
	drawTriangle(1);
	drawTriangle(5);
	drawTriangle(6);
	return 0;
}
