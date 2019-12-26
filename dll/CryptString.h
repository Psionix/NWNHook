#include <iostream>

char CharEncrypt(char c);
char CharDecrypt(char c);
void Decrypt(char *str);

#define EncryptString(string, new_string)	char new_string[sizeof(string)]; \
	if (string[0] != 0) { new_string[0] = CharEncrypt(string[0]); enc_step_00(string, new_string, 0 + 1); } else { new_string[0] = 0; } \
	Decrypt(new_string);

#define enc_step_00(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_01(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_01(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_02(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_02(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_03(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_03(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_04(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_04(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_05(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_05(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_06(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_06(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_07(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_07(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_08(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_08(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_09(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_09(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_10(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_10(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_11(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_11(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_12(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_12(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_13(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_13(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_14(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_14(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_15(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_15(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_16(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_16(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_17(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_17(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_18(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_18(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_19(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_19(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_20(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_20(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_21(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_21(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_22(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_22(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_23(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_23(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_24(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_24(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_25(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_25(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_26(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_26(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_27(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_27(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_28(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_28(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_29(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_29(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_30(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_30(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_31(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_31(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_32(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_32(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_33(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_33(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_34(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_34(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_35(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_35(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_36(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_36(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_37(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_37(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_38(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_38(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_39(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_39(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_40(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_40(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_41(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_41(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_42(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_42(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_43(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_43(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_44(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_44(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_45(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_45(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_46(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_46(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_47(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_47(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_48(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_48(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_49(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_49(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_50(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_50(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_51(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_51(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_52(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_52(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_53(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_53(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_54(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_54(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_55(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_55(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_56(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_56(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_57(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_57(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_58(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_58(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_59(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_59(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_60(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_60(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_61(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_61(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_62(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_62(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_63(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_63(string, new_string, i)	if (string[i] != 0) { new_string[i] = CharEncrypt(string[i]); enc_step_64(string, new_string, i + 1); } else { new_string[i] = 0; }
#define enc_step_64(string, new_string, i)	new_string[i] = 0;


