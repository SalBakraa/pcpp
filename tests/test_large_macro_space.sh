#!/usr/bin/sh

test_case_name='test_large_macro_space case: large set of undefined macros'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef test_1
#endif
#ifdef test_2
#endif
#ifdef test_3
#endif
#ifdef test_4
#endif
#ifdef test_5
#endif
#ifdef test_6
#endif
#ifdef test_7
#endif
#ifdef test_8
#endif
#ifdef test_9
#endif
#ifdef test_10
#endif
#ifdef test_11
#endif
#ifdef test_12
#endif
#ifdef test_13
#endif
#ifdef test_14
#endif
#ifdef test_15
#endif
#ifdef test_16
#endif
#ifdef test_17
#endif
#ifdef test_18
#endif
#ifdef test_19
#endif
#ifdef test_20
#endif
#ifdef test_21
#endif
#ifdef test_22
#endif
#ifdef test_23
#endif
#ifdef test_24
#endif
#ifdef test_25
#endif
#ifdef test_26
#endif
#ifdef test_27
#endif
#ifdef test_28
#endif
#ifdef test_29
#endif
#ifdef test_30
#endif
#ifdef test_31
#endif
#ifdef test_32
#endif
#ifdef test_33
#endif
#ifdef test_34
#endif
#ifdef test_35
#endif
#ifdef test_36
#endif
#ifdef test_37
#endif
#ifdef test_38
#endif
#ifdef test_39
#endif
#ifdef test_40
#endif
#ifdef test_41
#endif
#ifdef test_42
#endif
#ifdef test_43
#endif
#ifdef test_44
#endif
#ifdef test_45
#endif
#ifdef test_46
#endif
#ifdef test_47
#endif
#ifdef test_48
#endif
#ifdef test_49
#endif
#ifdef test_50
#endif
#ifdef test_51
#endif
#ifdef test_52
#endif
#ifdef test_53
#endif
#ifdef test_54
#endif
#ifdef test_55
#endif
#ifdef test_56
#endif
#ifdef test_57
#endif
#ifdef test_58
#endif
#ifdef test_59
#endif
#ifdef test_60
#endif
#ifdef test_61
#endif
#ifdef test_62
#endif
#ifdef test_63
#endif
#ifdef test_64
#endif
#ifdef test_65
#endif
#ifdef test_66
#endif
#ifdef test_67
#endif
#ifdef test_68
#endif
#ifdef test_69
#endif
#ifdef test_70
#endif
#ifdef test_71
#endif
#ifdef test_72
#endif
#ifdef test_73
#endif
#ifdef test_74
#endif
#ifdef test_75
#endif
#ifdef test_76
#endif
#ifdef test_77
#endif
#ifdef test_78
#endif
#ifdef test_79
#endif
#ifdef test_80
#endif
#ifdef test_81
#endif
#ifdef test_82
#endif
#ifdef test_83
#endif
#ifdef test_84
#endif
#ifdef test_85
#endif
#ifdef test_86
#endif
#ifdef test_87
#endif
#ifdef test_88
#endif
#ifdef test_89
#endif
#ifdef test_90
#endif
#ifdef test_91
#endif
#ifdef test_92
#endif
#ifdef test_93
#endif
#ifdef test_94
#endif
#ifdef test_95
#endif
#ifdef test_96
#endif
#ifdef test_97
#endif
#ifdef test_98
#endif
#ifdef test_99
#endif
#ifdef test_100
#endif
#ifdef test_101
#endif
#ifdef test_102
#endif
#ifdef test_103
#endif
#ifdef test_104
#endif
#ifdef test_105
#endif
#ifdef test_106
#endif
#ifdef test_107
#endif
#ifdef test_108
#endif
#ifdef test_109
#endif
#ifdef test_110
#endif
#ifdef test_111
#endif
#ifdef test_112
#endif
#ifdef test_113
#endif
#ifdef test_114
#endif
#ifdef test_115
#endif
#ifdef test_116
#endif
#ifdef test_117
#endif
#ifdef test_118
#endif
#ifdef test_119
#endif
#ifdef test_120
#endif
#ifdef test_121
#endif
#ifdef test_122
#endif
#ifdef test_123
#endif
#ifdef test_124
#endif
#ifdef test_125
#endif
#ifdef test_126
#endif
#ifdef test_127
#endif
#ifdef test_128
#endif
#ifdef test_129
#endif
#ifdef test_130
#endif
#ifdef test_131
#endif
#ifdef test_132
#endif
#ifdef test_133
#endif
#ifdef test_134
#endif
#ifdef test_135
#endif
#ifdef test_136
#endif
#ifdef test_137
#endif
#ifdef test_138
#endif
#ifdef test_139
#endif
#ifdef test_140
#endif
#ifdef test_141
#endif
#ifdef test_142
#endif
#ifdef test_143
#endif
#ifdef test_144
#endif
#ifdef test_145
#endif
#ifdef test_146
#endif
#ifdef test_147
#endif
#ifdef test_148
#endif
#ifdef test_149
#endif
#ifdef test_150
#endif
#ifdef test_151
#endif
#ifdef test_152
#endif
#ifdef test_153
#endif
#ifdef test_154
#endif
#ifdef test_155
#endif
#ifdef test_156
#endif
#ifdef test_157
#endif
#ifdef test_158
#endif
#ifdef test_159
#endif
#ifdef test_160
#endif
#ifdef test_161
#endif
#ifdef test_162
#endif
#ifdef test_163
#endif
#ifdef test_164
#endif
#ifdef test_165
#endif
#ifdef test_166
#endif
#ifdef test_167
#endif
#ifdef test_168
#endif
#ifdef test_169
#endif
#ifdef test_170
#endif
#ifdef test_171
#endif
#ifdef test_172
#endif
#ifdef test_173
#endif
#ifdef test_174
#endif
#ifdef test_175
#endif
#ifdef test_176
#endif
#ifdef test_177
#endif
#ifdef test_178
#endif
#ifdef test_179
#endif
#ifdef test_180
#endif
#ifdef test_181
#endif
#ifdef test_182
#endif
#ifdef test_183
#endif
#ifdef test_184
#endif
#ifdef test_185
#endif
#ifdef test_186
#endif
#ifdef test_187
#endif
EOF

cat << EOF > expected_output
#ifdef test_1
#endif
#ifdef test_2
#endif
#ifdef test_3
#endif
#ifdef test_4
#endif
#ifdef test_5
#endif
#ifdef test_6
#endif
#ifdef test_7
#endif
#ifdef test_8
#endif
#ifdef test_9
#endif
#ifdef test_10
#endif
#ifdef test_11
#endif
#ifdef test_12
#endif
#ifdef test_13
#endif
#ifdef test_14
#endif
#ifdef test_15
#endif
#ifdef test_16
#endif
#ifdef test_17
#endif
#ifdef test_18
#endif
#ifdef test_19
#endif
#ifdef test_20
#endif
#ifdef test_21
#endif
#ifdef test_22
#endif
#ifdef test_23
#endif
#ifdef test_24
#endif
#ifdef test_25
#endif
#ifdef test_26
#endif
#ifdef test_27
#endif
#ifdef test_28
#endif
#ifdef test_29
#endif
#ifdef test_30
#endif
#ifdef test_31
#endif
#ifdef test_32
#endif
#ifdef test_33
#endif
#ifdef test_34
#endif
#ifdef test_35
#endif
#ifdef test_36
#endif
#ifdef test_37
#endif
#ifdef test_38
#endif
#ifdef test_39
#endif
#ifdef test_40
#endif
#ifdef test_41
#endif
#ifdef test_42
#endif
#ifdef test_43
#endif
#ifdef test_44
#endif
#ifdef test_45
#endif
#ifdef test_46
#endif
#ifdef test_47
#endif
#ifdef test_48
#endif
#ifdef test_49
#endif
#ifdef test_50
#endif
#ifdef test_51
#endif
#ifdef test_52
#endif
#ifdef test_53
#endif
#ifdef test_54
#endif
#ifdef test_55
#endif
#ifdef test_56
#endif
#ifdef test_57
#endif
#ifdef test_58
#endif
#ifdef test_59
#endif
#ifdef test_60
#endif
#ifdef test_61
#endif
#ifdef test_62
#endif
#ifdef test_63
#endif
#ifdef test_64
#endif
#ifdef test_65
#endif
#ifdef test_66
#endif
#ifdef test_67
#endif
#ifdef test_68
#endif
#ifdef test_69
#endif
#ifdef test_70
#endif
#ifdef test_71
#endif
#ifdef test_72
#endif
#ifdef test_73
#endif
#ifdef test_74
#endif
#ifdef test_75
#endif
#ifdef test_76
#endif
#ifdef test_77
#endif
#ifdef test_78
#endif
#ifdef test_79
#endif
#ifdef test_80
#endif
#ifdef test_81
#endif
#ifdef test_82
#endif
#ifdef test_83
#endif
#ifdef test_84
#endif
#ifdef test_85
#endif
#ifdef test_86
#endif
#ifdef test_87
#endif
#ifdef test_88
#endif
#ifdef test_89
#endif
#ifdef test_90
#endif
#ifdef test_91
#endif
#ifdef test_92
#endif
#ifdef test_93
#endif
#ifdef test_94
#endif
#ifdef test_95
#endif
#ifdef test_96
#endif
#ifdef test_97
#endif
#ifdef test_98
#endif
#ifdef test_99
#endif
#ifdef test_100
#endif
#ifdef test_101
#endif
#ifdef test_102
#endif
#ifdef test_103
#endif
#ifdef test_104
#endif
#ifdef test_105
#endif
#ifdef test_106
#endif
#ifdef test_107
#endif
#ifdef test_108
#endif
#ifdef test_109
#endif
#ifdef test_110
#endif
#ifdef test_111
#endif
#ifdef test_112
#endif
#ifdef test_113
#endif
#ifdef test_114
#endif
#ifdef test_115
#endif
#ifdef test_116
#endif
#ifdef test_117
#endif
#ifdef test_118
#endif
#ifdef test_119
#endif
#ifdef test_120
#endif
#ifdef test_121
#endif
#ifdef test_122
#endif
#ifdef test_123
#endif
#ifdef test_124
#endif
#ifdef test_125
#endif
#ifdef test_126
#endif
#ifdef test_127
#endif
#ifdef test_128
#endif
#ifdef test_129
#endif
#ifdef test_130
#endif
#ifdef test_131
#endif
#ifdef test_132
#endif
#ifdef test_133
#endif
#ifdef test_134
#endif
#ifdef test_135
#endif
#ifdef test_136
#endif
#ifdef test_137
#endif
#ifdef test_138
#endif
#ifdef test_139
#endif
#ifdef test_140
#endif
#ifdef test_141
#endif
#ifdef test_142
#endif
#ifdef test_143
#endif
#ifdef test_144
#endif
#ifdef test_145
#endif
#ifdef test_146
#endif
#ifdef test_147
#endif
#ifdef test_148
#endif
#ifdef test_149
#endif
#ifdef test_150
#endif
#ifdef test_151
#endif
#ifdef test_152
#endif
#ifdef test_153
#endif
#ifdef test_154
#endif
#ifdef test_155
#endif
#ifdef test_156
#endif
#ifdef test_157
#endif
#ifdef test_158
#endif
#ifdef test_159
#endif
#ifdef test_160
#endif
#ifdef test_161
#endif
#ifdef test_162
#endif
#ifdef test_163
#endif
#ifdef test_164
#endif
#ifdef test_165
#endif
#ifdef test_166
#endif
#ifdef test_167
#endif
#ifdef test_168
#endif
#ifdef test_169
#endif
#ifdef test_170
#endif
#ifdef test_171
#endif
#ifdef test_172
#endif
#ifdef test_173
#endif
#ifdef test_174
#endif
#ifdef test_175
#endif
#ifdef test_176
#endif
#ifdef test_177
#endif
#ifdef test_178
#endif
#ifdef test_179
#endif
#ifdef test_180
#endif
#ifdef test_181
#endif
#ifdef test_182
#endif
#ifdef test_183
#endif
#ifdef test_184
#endif
#ifdef test_185
#endif
#ifdef test_186
#endif
#ifdef test_187
#endif
EOF

build/pcpp --process-all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
