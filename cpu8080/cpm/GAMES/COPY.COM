1U :f >�2f ͚!��r1U >�2 >2 !�!��r� ˯�C���B�S�O�M�E �2f �  > 2 !�r� ˯�C���I�)�D�Y�V�d�Sʇ�Fʬ�T���B�S�Xʅ�E�F!m�r��J�*�2 !�^"  !�
~����R���#¯>2 ��>��R��<� ��!��r�J�*�2 ��!�^"  !�
~����W���#��>2 ��>��W��<� ��2 >2 !�
~���!�z"  �R�R��#�2>2 >��: �����>2 > �!m�r!o�r� ˯�(!m�r�!�r�J�*�2 ��!G�r��!U "  ���R��<�
8���!U4"  ���R��<�
8���: <�(ڏ�  �C	 U �S !U4" ~G���K	 �C	 >��* #" �[ �S �!�r!I�r�K	 Hͣ�K	 ͣ!W�r* ~Oͣ!a�r�[ Oͣ!�r�!m�r!��r� ˯�(!m�r�!�r�����2\_���!m�r!E�r� ˯�(!m�r�!�r�!m�r!P�r� ˯�(!m�r�!�r�!m�r!��r� ˯�(!m�r�!�r�?!m�r!��r� ˯�(!m�r�!�r>�22ò!m�r!*�r� ˯�(!m�r�!�r>�2> 2Y!m�r!��r� ˯�(!m�r�!�r> 22+!m�r!��r� ˯�(!m�r�!�r́����J�*�2 ��!G�r��!U >;!U U4�a�)�����a�P: <�(ڹ:����!u�r�!��rͫ́����J�*>2 ��!U "  > ��R�<� �U4�!U @�~ � ,�~	�C %�~
�O �~�M �~� ���#� ��> ��  ���>$:U �$�!��r[_
́:\_!]_G�(~�a8�{0˯w#�w�!U4�r!�r!�r�"  2 ͬ" ~���R��#: =2 ���:� ��: O��͂>
2 ͬ�* U  ���	�#����S  " ~���W��#: =2 ��[  �>
2 ~���R��#: =2 �9���������J�: o�*   {��(�v���qv���w�v��v��"  �"��(-(�*   ��B�>��ɯ��J>�! �v��¹�"�!�~�: ��~� 	 	 	
��(=���J�>�������: �>��z���v��G ���J�������> ��"����> ��"���w�˷�2�z��X�� ��#x��d��y0�d8��d�{�0(́�0�
8��:�́�_́����y�G��	�	�	�	y����x������
���7_́��0�!�r�K �p�!G�r!�r��!�r�O�p!(�r!�r: ���^�����G(�!G�r!�r�!G�r!�r��!/�r!�r: ���^���!G�r!�r�!?�r� ˯���~�$��_́�#�� �� �����J�
KAYPRO Double density diskette copy program.     Single sided V1.5

$

C  COPY   This option is used to copy a diskette in drive A to a diskette
          in drive B.  The diskette in drive B will be formatted (erased)
          during the copy operation.  At the end of the operation, a copy
          of CP/M will be placed on the diskette in drive B.

B  BLANK  This option is used to make a blank diskette.   The diskette in
          drive B will be formatted (erased); then a copy of CP/M will be
          placed on the diskette.

O  OTHER  This is a list of other options for use by  programmers.  These
          options produce more complex error messages and assume  techni-
          cal expertise.

E  EXIT   To get out of this program.  Be sure a diskette with CP/M on it
          is in drive A!

Please enter selection.  Press C, B, O, or E ==>$

Use the "ESC" key to terminate an operation in progress.
Note:  These options do not terminate execution on the first encountered error.

C  COPY        Format B, copy, verify, and sysgen.
B  BLANK       Make a blank diskette {format, sysgen}.
I  Image       Format B, copy, and verify.
D  Duplicate   Copy and verify.
V  Verify      Verify that A and B are the same.
S  Sysgen      Write system image only.
F  Format B    Format (erase) the diskette in drive B.
X  Format A    Format (erase) the diskette in drive A.
T  Track       Format a single track on drive B.
R  Return      Return to first menu.
E  Exit        BEFORE pressing E, place a system diskette in A.

Selection (C,B,I,D,V,S,F,T,E) ==>$
Writing CP/M to the diskette in drive B.
$
$Track $ Sector $Drive A $Drive B $ {BAD}$ Memory buffer cmpr err$$Drive B byte $ data is $ should be $$

Verify that the diskette in drive A is the same as the diskette in drive B.

Push RETURN to start, any other key to abort.$

Make a blank diskette.

This  option  will prepare a new or previously-used diskette  to  receive
data  by ERASING WHATEVER DATA IS ON IT,  formatting it into  tracks  and
sectors, and putting a copy of CP/M on a portion of the diskette reserved
for it. During this process, the diskette will be checked for errors.

To use this option:
1.  Place into drive B a diskette which you want to prepare to receive data.
2.  Press the RETURN key.

All information on the diskette will be erased,  and the diskette will be
prepared to receive data.

Push RETURN to start, any other key to abort.$

If you continue you will format the diskette in drive A!

Push RETURN to start, any other key to abort.$

Copy the diskette in drive A to drive B.

This  option  is used to copy diskettes.  During the copy  operation  the
diskette in drive B is checked for errors and the copy is verified.

To use this option:
1.   Place the original (master or source) diskette into drive A.
2.   Select  either a new diskette or a used diskette that may be  erased
     (i.e.   have  the data on the diskette in drive A recorded over it).
     Place this diskette into drive B.

When you have placed the diskettes into the drives,  press the RETURN key
to start the copy operation.  You may return to the main menu by pressing
any other key.$

Copy a diskette.  Do Format B, Copy, and Verify.

Push RETURN to start, any other key to abort.$

Copy a diskette.  Do Copy and Verify.

Push RETURN to start, any other key to abort.$

Write the CP/M image in RAM onto the diskette in drive B

Push RETURN to start, any other key to abort.$

Format the diskette in drive B.

Push RETURN to start, any other key to abort.$

Format a single track on the diskette in drive B.  You will be asked for
the track number if you continue.
Push RETURN to start, any other key to abort.$
You have had an error, push the ESC key to exit.
$     Under normal operation,  when you push the reset button,  you see an
A> prompt. If you would like to bypass the A> prompt, type in the name of
the program you want to use.  A list of programs contained on the disk is
given below.  Some names are abbreviated, because a name may have no more
than 8 characters.  If there is a master control program, such as MENU or
HELLO,  you may want to select it.  If you want one of these programs  to
execute when you push the RESET button,  just enter its name when you are
asked for a command line.

$       << There are no programms that can be run on this diskette >>

$Please  enter an initial command line (name).  This command line  is  not
checked  to  ensure it is correct.  This allows greater  flexibility  for
experienced users who understand that this is a CCP command line.  If you
push RETURN only you will get the normal A> prompt. ==>$��  !<�r�7	!<�r�2�< �y2 !��r����E��͂y�( �>��!��r�����!��r����!��r� ˯���y2 �2�<�E��!<�r͂��: �  ����*�|�! "�|�e�U�"�|�˗���e> �%� !> �%�(> �%�(> �%� 	� �������!��!}
P 6N#z� � 6 #z� � 6�#z� � 6�#z� � 6N#z� � 6 #z� � 6�#z� � 6�#z� �q# 6 #z� ��~#�w# 6#z� � 6�#z� � 6N#z� � 6 #z� � 6�#z� � 6�#z� � 6�#z� � 6�#z� � 6N#z� �� 6N#z� ��}�R"�|��� 	��!�O 	~��� �� ��0��:��0Gy�����O��y0�d8��d�{�0(́�0�
8��:�́�_́��>��>���J> ���J��y�>�� �_��G ���b�����J��*�|�!}:f �>�2f >��v��z��2f ��

Format a single track of the diskette
Enter track number (<ESC> key to abort)  xx$
Formatting track:$ Press RETURN key to format, any other key to abort $ Sector $
$  �͖'�H  �r�c �rԈܘ�rÈ͘�rԈܘ�!�	N#F#^#V#~#fo|��}��	�	+x��~+Íx��w#ØA�* |U  ��ͅ��L�!O��>,�P*8& 2M^#V���y�������!JF#~#�P��>(�P��>)�P��