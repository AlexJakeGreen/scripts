!  9"y�4� �K͞�+�����'�+�FÎ�r�*y��* ���   w#w#w��������^#V#�*~#fo��^#� *~#fo��^#V#�*n��^#� *n��^#V#��	~#fo��^#��& 	~#fo�! �+�!  �#�! �+�!  �#�! �+�! �+� ��  �� ��  �� �� ��}��|���z��{���|��}���|���z��7��|���|��7�z��Z��Z��)��|/g}/o#�  |����͉��k|/g}/o#�ɯ2q�Z��Z�k:q�|/g}/o#�|��/g}/o#:q<2q���q��DM!  x��xGyO҃�)��v|���͔`i��|���)Ö�x  ���ڷ�z/W{/_ѯzW{_�=��yOxGæ�2q�Z��Z�͉�M|���}���ȯ|g}o����)��|/g}/o#�z/W{/_�! 9~#fo�! �! �!
 �! �! �! ��! 9�~#�A���"s!`�*"�!�"�!�Y"�>�2�>�2�>�2�2�! �"�! "�!@ "�! "�� � ʞ� !� �F#x�± �~#±�!� b�2r~#� �����"��2r��+}|~#���G:r�x���"���2r+w#� ��+6 #���!�6 #��2w2x*s�!���>��r�<o& ��F��=��-�/�`�
�r�'~��h������ ��6 ��!  �+��`W���?��_ !�~�7�z�?��`����:> ª��@w#�G�.¶���w w#�?��*>?������w#��������> w#������.7��:7��7��!��a��{�� �� ��	����`�O�E�!��y�6���$ ��-7���r�B�o& ))T])))�!�y������� � �����               ##�� ��!2 	s#r�!C��	 ��͐*###�� ��!4 	s#r�!f��	 ��͐*####�� ��!6 	s#r�!���	 ��͐* �� �#�!8 	s#r�!���	 ��͐* �� �#�!< 	s#r�!���	 ��͐*	 �� ��!> 	s#r�!���	 ��� �!A 9����

CP/M Data Program Version 1.0 by C. B. Mueller 12/15/82

 This program will print CP/M information based on your system  size
and attributes of the drive that it is run on.

 %x = CCP Start Address
 %x = BDOS Start Address
 %x = BIOS start Address (Cold Boot)

 BIOS ENTRY ADDRESS
 %x = Warm Boot		%x = Select Disk
 %x = Console Stat		%x = Set Tra�!��9�DM! �͛��`is#r͐ ###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�!
 	s#r͐
###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r͐###�! 	s#r!W����!�����!�����͐ ���!�����͐ ���! �����͐ +++�!9�����!_����͐�͐ �!s������͐�͐�!�������͐�͐�!�������͐�͐�!�������͐�͐�!������͐�͐
�!8������͐�͐�!\������͐�͐�!�������! ��/��!$ 	s#r͐$|�¬	!@ 	6Aò	!@ 	6B!  �͐$�!  �!  �͐��6������!  	s#r!@ 	n& �!������͐ �!������͐ �͛��!& 	s#r�!������͐  �͛��!( 	s#r�!�����͐ 
 �͛��!* 	s#r�!B�����͐  �͛��!, 	s#r�!e�����!�����͐*��/��!0 	s#r�!������͐*##��/��!2 	s#r�!������͐*###��/��!4 	s#r�!������͐*####��/��!6 	s#r�!�����͐* �͛�#�!8 	s#r�!3�����͐* ��/�#�!< 	s#r�!V�����͐*	 ��/��!> 	s#r�!y������T�!A 9����

CP/M Data Program Version 1.0 by C. B. Mueller 12/15/82

 This program will print CP/M information based on your system  size
and attributes of the drive that it is run on.

 %x = CCP Start Address
 %x = BDOS Start Address
 %x = BIOS start Address (Cold Boot)

 BIOS ENTRY ADDRESS
 %x = Warm Boot		%x = Select Disk
 %x = Console Stat		%x = Set Track Number
 %x = Console Input		%x = Set Sector number
 %x = Console Output		%x = Set DMA Address
 %x = List Output		%x = Read Disk
 %x = Punch Output		%x = Write Disk
 %x = Reader Output		%x = Return List Status
 %x = Home Drive		%x = Sector Translate

 DRIVE %c DISK PARAMETER HEADER TABLES
 Disk Parmater Header Address = %x
 Translation Table Address    = %x
 Directory Buffer Address     = %x
 Disk Parameter Block Address = %x
 Disk Allocation Vector       = %x

 DISK %c PARAMETER BLOCK TABLES
 Logical Sectors per Track   = %dD
 Block Shift Factor          = %dD
 Block Mask                  = %x
 Extent Mask                 = %x
 Max Disk Blocks             = %dD
 Max Directory Entries       = %dD
 Directory Block Alloc       = %x
 �!��9�DM͐
��/��`is#r͐
#��/��! 	s#r͐ �?�͐ ��! 	s#r͐���! 9�����!y�9�DM!� 	�`i����`i��r��!� 9�����!h�9�DM!� 	^#Vr+s�~#fo�! 	s#r͐��!� 	s#r! 	^#Vr+s�n�`is{��c`in}�%�M! 	�!� 	s#r!� 	6#6 !� 	 s!� 	s!� 	s͐n}�-¢! 	^#Vr+s!� 	4͐n}�0²!� 	4͐n& ��@�}���! 	��o���!  �!� 	s#r! 	^#Vr+s�n�`is{�.�! 	��o��!� 	s#r!� 	4! 	^#Vr+s�n�`is`in& ����}�D�F�Uʂ�Xʋ�Oʔ�C���S��7͐�~#fo|҂!� 	^#Vr+s�6-͐��͐�~#fo����s#r!� 	^#Vr+s! 	6
Ú! 	6Ú! 	6!� 	�~#fo�! 	n& �!� 	^#Vr+s�~#fo�!� 	�͞���ѯg����s#rÃ!� 	^#Vr+s�!� 	^#Vr+s�~#fo��s!� 	^#Vr+sÃ!� 	n}��!� 	6�#6 !� 	^#Vr+s�~#fo�! 	s#r͐n}�ʃ͐�|�ʃ!� 	^#Vr+s�! 	^#Vr+s�n��s!� 	^#Vr+s!� 	^#Vr+s�9͐�6 ! 	�!� 	s#r!� 	n}���!� 	^#Vr+s!��|��!� 	^#Vr+s�!� 	n}���!0 ��!  ��sß͐��!� 	^#Vr+s�n��s{��!� 	^#Vr+s��!� 	n}��4!� 	^#Vr+s!��|�4!� 	^#Vr+s�6 ��J!� 	^#Vr+s�`in��s�`!� 	^#Vr+s�`in��s�A͐�6 �!� 9�����!  9�DM͐n}�ʜ! 	^#Vr+s�n& ��W��z���!��9�DM͐�͐	�����͐^#Vr+s�͐����͐0 ��͐7 ��s! & �7͐	�͐�͐	�͉�͐�͞����`is͐	�͐�͐	��)�͐�͞���`in& #& �7�! 9�����!  9�DM! 	n& ��|ͯ�k! 	n& ��|ͩ�����!��9�DM`i�w#w͐~#fon& ��@�}�ʼ͐ 
 �?�͐^#Vr+s�nѯg���`is#r�}͐ ���! 9�����!  9�DM! 	n& �� �|���! 	n& ����! 	n& & �����!  9�DM! 	n& ��|ͯ�+! 	n& ��|ͩ�����
n& ��7�*��*�DM:�!R�*��*����� �
��
�k� >
_� � ��z��� ��  ��x = List Output		%x = Read Disk
 #%x = Punch Output		%x = Write Disk
 ,%x = Reader Output		%x = Return List Status
 (%x�  ��x = List Output		%x = Read Disk
 #%x = Punch Output		%x = Write Disk
 ,%x = Reader Output		%x = Return List Status
 (%x