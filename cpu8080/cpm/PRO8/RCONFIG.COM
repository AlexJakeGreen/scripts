�k             (  � ? �    � � ( � ? �    � � (  o� �    `Q(  �� �    ������_� ��	9� 	W� � ˯�A8��F0��A2��
:"(��(>�2�2> 2�	� � ˯�Y(�>:h$��(>�2�!$  ��	9� 	y� � �	~� ��0ʲ�1���2ʾ	�3��4�+�5�x�6���7ʱ�8��9����L��:���	�� > 2�	� �� � (�����
�m� �:���G� �+:�<2��(�	� �    50
   75
  110     Move the cursor to the baud rate which
  134.5
  150     you want to use by typing the [LINE FEED] key.
  300
  600
 1200     When the cursor is at the rate you want
 1800
 2000     type the RETURN key and it will be set.
 2400
 3600     The new baud rate will be effective now.
 4800
 7200
 9600
19200
Type [ESC] key to return to the main menu (no changes)

$$
$�+:�����:�<2��M 	�
��*���  :�=_*�yw��� � (��
����08��:8˯�G0��A8��_� ���_� ��0��������� � (��
(� ���08��:8˯�G0��A8��_� ���_� ��0�O�O�<�!�"�!V "�	�� > 2�p	�\�)u	�\�)z	�\�)	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�)�	�\�� � (�����
 ���-----------------------------	Move the cursor to the key you wish
:      :      :      :      :
:   7  :   8  :   9  :   -  :	to change. Use the [LINE FEED] key.
:      :      :      :      :
:  37  :  38  :  39  :  2D  :	The default HEXADECIMAL code will be
;------;------;------;------:
:      :      :      :      :	the number which is flashing.
:   4  :   5  :   6  :   ,  :
:      :      :      :      :
:  34  :  35  :  36  :  2C  :	When you reach the key you wish to change.
:------:------:------:------:
:      :      :      :      :	Type in the new HEXADECIMAL code.
:   1  :   2  :   3  :      :
:      :      :      :      :
:  31  :  32  :  33  : ENTER:	(* NOTE: you must enter both digits *)
:------:------:------:      :
:             :      :  0D  :	(*       to change the code.        *)
:      0      :   .  :      :
:             :      :      :
:     30      :  2E  :      : Type the [ESC] key to return to the previous menu
-----------------------------
$=3&�=.#�=.*�=.1�=)#�=)*�=)1�=$#�=$*�=$1�=$8�=)8�=08�=31�=3I��:���!R "�!�"�	%
� > 2���\�)��\�)��\�)��\�)��\�� � (��
��	� �����	þ	

            ---------------------------------
            :       :       :       :       :
            :   ^   :   :   :  <-   :  ->   :
            :   :   :   v   :       :       :
            :       :       :       :       :
            :  0B   :  0A   :  08   :  0C   :
            ---------------------------------


       Type the [ESC] key to return to the previous menu.

 Move the cursor to the key which you wish to change by pressing
 the [LINE FEED] key. The default HEXADECIMAL code will be the flashing number.
 Type the new HEXADECIMAL number you wish to assign to the key.

 You must type both digits to effect the change.

$='/�='7�='?�='G�=+2�:���	�� !P "�!�"�> 2�_�\�9d�\�9i�\�9n�\�9s�\�� � (�����
 ����� � (�� �*���
 :�<2��� �:��  _!r^#V��[��z���ˏ��!
>2���ˇˏ:�<2��˷��!�˿˷���

	Default settings	Posibile changes
	----------------	----------------

	CON:=CRT:		CON:=TTY:	Type the [LINE FEED] key to 
	LST:=LPT:		LST:=TTY:	move the cursor to the mode 
	PUN:=TTY:		- none -	you wish to select.
	RDR:=TTY:		- none -	Then the RETURN key to enter
						your selection.
	Type the [ESC] key to return to the previous menu.

* CON: If you chose CON:=TTY: then all input and output  will be through the
	serial connector on the back of your KAYPRO II instead of through the
	keyboard and the CRT.


* LST:	This setting will decide wether the output directed at a printer
	will go through the LPT: (parallel connector) or to the
	TTY: (serial connector).
$=%,�=%D�=&,�=&D�=*3�:���	� � ����18��90��1!    (= ��!d s#r��>�2�	G� � � >2�	�� � ˯�Y :���2�:�2��� >�2�	�� ��(���(2�>�2���2��>�2��2�	�� � ����18��60��1 
:���!�(�2 
:���!��3 
:���!��4 :���!"2�><w�	i� � �1 :����2 :����3 �:�����2�� > 2�!%( > w#���>�2�	�� � ����0�0�2	C� � �0�0�2�:����	�� � ˯�C8��Q0��A2f 	/� � ˯�C8��Q0��A2g ��>�2�!$"��z	'� � ˯�Y(�  :���4! "�> 2��z�4:�O�� �C� �C�>2�!� "��K����K����K���:��(����� ((:��(
	�� 	�� 	�� � ��*��[�"�:�<2�:�=2�� �:�O��  ��  ��(����� �	�� ��>22#(:��((! �(= �-( ��:��(=(##2( ��:���(2)(:��(2&(:���(2*(:��(2,(:�2'(:�O��  �� ��(�������	�� �   �� �� ��! ��$ ��' ��* ���                 ������	� �� � (�����  


BIOS changes are only allowed on a 63K system disk.

Enter [ESC] to return to menu or any other key to exit.

$

1.............. Drive A is slow  
2.............. Drive B is slow  
3.............. Drive C is slow  
4.............. Drive D is slow  
5.............. Select Step Rate


Selection (1,2,3,4, or 5) ===> $

Choose steprate for marked drive(s)

Drive Size >   8 inch         5 inch
1 .........     6 ms           12 ms
2 .........    10 ms           20 ms
3 .........    15 ms           30 ms

Selection (1,2, or 3) ===> $


Enter the character you wish to have for your cursor.
(Enter carriage return for a block cursor)

Waiting for a character ===> $
Do you want the cursor to blink (Y or N) ? $


Enter the key you wish to use to dump the screen  to printer
( The data keypad produces unique characters and may be used for this function)

Waiting for input ========> $

Set Ram Disk Size 

 0 --- 256K Ram Disk

 1 --- 512K Ram Disk

 2 --- 768K Ram Disk

 3 --- 1024K Ram Disk

Select (0,1,2, or 3) -------> $


Set Printer Buffer Size

 0 --- 4K printer buffer

 1 --- 32K printer buffer

 2 --- 64K printer buffer

Select (0,1, or 2) ---------> $


Select Ram Disk Position
Should be your last drive +1

Select (C-P) ---------------> $


Select  Ram Disk <=> Drive A: swap letter

Select (C-P) ---------------> $
$


DONE WITH CONFIGURATION
$
Please type any key to continue$
Bad Sector On Destination Disk 
$
Bad Sector On Source Disk 
$

The time it takes for your disk drives to shut off is programmable with
this bios.

A 1 would give you 2 seconds at 2.5 MHz or 1 second at 5 MHz.
An 8 would give you 16 seconds at 2.5 MHz or 8 seconds at 5 MHz.

Enter number from 1-8  (or [ESC])  Selection ? -------> $

Install NEW BIOS (Y or N) ? $

Pro8+ Ram Disk Configuration Program   14-Aug-85  [DmC]

$

0 --------> Erase Pro8 configurations
1 --------> I/O Byte cold boot defaults
2 --------> Cursor Keys definitions
3 --------> Numerical keypad definitions
4 --------> Set Baud rate
5 --------> Set Disk Drive Time Out
6 --------> Set Screen Dump Character
7 --------> Set Cursor Character
8 --------> Set Steprate for Slow Drives
9 --------> Set Ram Disk Size
[ESC] key ----> EXIT this program 

The changes will not take effect until you reset your Kaypro.
Upon exiting this program you will be asked whether you 
want to write the changes to disk.
The Destination Disk must have 63K system tracks for selections 1-5.

Selection ( 0,1,2,3,4,5,6,7,8,9 or [ESC] )====> $

Write the changes to disk ? ( Y or N ) ===> $


This  program allows you to set a default configuration for
your CP/M system disk. It contains an improved BIOS for 63K
systems.  If you are already running this bios, the bios on
disk will be read and modified. Otherwise the new BIOS will
be installed.  BIOS changes will only work for 63K systems.
You may set the PRO8 configurations on any size system with
this same program.  


Enter the drive letter of the disk you want to modify ==> $�L�ã����!��C��]��X��R�Ó�Ø�����������������{����� 
0123456789-,. ��͎��2 :3�2 �'�
Ram Disk 63k CP/M v 2.2
 *G�"��>�!�2  " !�2 " : O��� �O� �͎� ͘�͓��'�
Ram Disk Off E:
 1   ���! �"�,��������� �*�� "��2��y�>(����������*��+"��|�� �: �.3��.*�����(�: �.6��.-�����!5�O 	~�: �.9��.E��!6 ��.9��: ��.9��.?����.E�@��!9 ��: ��.B��.<������.��.��:J��(> �(:I�� :��O:I���3�.K:I�O:��2��>A :I��A2���  .,.(.$*G�"��.*G�"��.:4��(.!.' ������s��1�����& ��{���˿���~#��O�C��:���O��'�
H      ͵�|�:I�2�����!z� ��:I�<2��=O:���A2���==>                                                                            ͵�|�:I�2�����!z� 