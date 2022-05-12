#include <unity.h>

extern char Kp, Ti, Td;

void setUp(void)
{
	return;
}
void tearDown(void)
{
	return;
}

void test_reset_empty_string(void){
	// Asserts if module is detecting empty strings
	TEST_ASSERT_EQUAL_INT(-1, cmdProcessor());
	resetCmdString();
}

void test_new_char(void){
	// Introduce 10 characters to the string, assert if on the 11th insert it returns an error and not on the others
	
	for(int i=0; i < 10; i++){
		TEST_ASSERT_EQUAL_INT(0, newCmdChar('0'));	
	}	
	
	TEST_ASSERT_EQUAL_INT(-1, newCmdChar('0'));
	
	resetCmdString();
}

void test_return_invalid_cmd(void){
	// Introduce an invalid command and assert if it returns an error
	
	newCmdChar('#');
	newCmdChar('0');
	newCmdChar('0');
	newCmdChar('0');
	newCmdChar('0');
	newCmdChar('0');
	newCmdChar((unsigned char)('0'+'0'+'0'+'0'));
	newCmdChar('!');
	TEST_ASSERT_EQUAL_INT(-2, cmdProcessor());
	
	resetCmdString();
}

void test_wrong_cs(void){
	// Introduce a wrong checksum and assert if it returns an error
	
	newCmdChar('#');
	newCmdChar('P');
	newCmdChar('1');
	newCmdChar('2');
	newCmdChar('3');
	newCmdChar((unsigned char)('P'+'1'+'2'+'0'));
	newCmdChar('!');
	TEST_ASSERT_EQUAL_INT(-3, cmdProcessor());
	
	resetCmdString();
	
	newCmdChar('#');
	newCmdChar('S');
	newCmdChar((unsigned char)('D'));
	newCmdChar('!');
	TEST_ASSERT_EQUAL_INT(-3, cmdProcessor());
	
	resetCmdString();
}

void test_string_format_wrong(void){
	// Test if string format is wrong
	
	newCmdChar('#');
	newCmdChar('P');
	newCmdChar('1');
	newCmdChar('2');
	newCmdChar('3');
	newCmdChar((unsigned char)('T'+'1'+'2'+'3'));
	newCmdChar('$');
	TEST_ASSERT_EQUAL_INT(-3, cmdProcessor());
	
	resetCmdString();
	
	newCmdChar('#');
	newCmdChar('P');
	newCmdChar('1');
	newCmdChar('2');
	newCmdChar('!');
	newCmdChar((unsigned char)('T'+'1'+'2'+'3'));
	newCmdChar('!');
	TEST_ASSERT_EQUAL_INT(-3, cmdProcessor());
	
	resetCmdString();
}

void test_set_variables(void){
	// Introduce a P command and assert if the values introduced are correct
	
	newCmdChar('#');
	newCmdChar('P');
	newCmdChar('1');
	newCmdChar('2');
	newCmdChar('3');
	newCmdChar((unsigned char)('P'+'1'+'2'+'3'));
	newCmdChar('!');
	cmdProcessor();
	TEST_ASSERT(Kp == '1');
	TEST_ASSERT(Ti == '2');
	TEST_ASSERT(Td == '3');
	
	resetCmdString();
}

void test_s_command(void){
	// Send a S command and assert if it returns no error
	
	newCmdChar('#');
	newCmdChar('S');
	newCmdChar((unsigned char)('S'));
	newCmdChar('!');
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	resetCmdString();
}

void test_p_command(void){
	// Send a P command and assert if it returns no error
	
	newCmdChar('#');
	newCmdChar('P');
	newCmdChar('1');
	newCmdChar('2');
	newCmdChar('3');
	newCmdChar((unsigned char)('P'+'1'+'2'+'3'));
	newCmdChar('!');
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	resetCmdString();
}

int main(void)
{
	UNITY_BEGIN();
	
	resetCmdString();
	
	RUN_TEST(test_reset_empty_string);
	RUN_TEST(test_new_char);
	RUN_TEST(test_return_invalid_cmd);
	RUN_TEST(test_wrong_cs);
	RUN_TEST(test_string_format_wrong);
	RUN_TEST(test_set_variables);
	RUN_TEST(test_s_command);
	RUN_TEST(test_p_command);
	
	return UNITY_END();
}
