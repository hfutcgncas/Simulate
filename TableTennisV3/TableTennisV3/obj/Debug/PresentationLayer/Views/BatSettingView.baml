   M S B A M L   `   `   `  ����H  DTableTennisV3, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]3clr-namespace:TableTennisV3.PresentationLayer.Views%TableTennisV3.PresentationLayer.Views  5      g8clr-namespace:TableTennisV3.PresentationLayer.ViewModels*TableTennisV3.PresentationLayer.ViewModels  5      G(clr-namespace:TableTennisV3.ToolResourceTableTennisV3.ToolResource  5	      :    4TableTennisV3.PresentationLayer.Views.BatSettingView   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      Elocal3clr-namespace:TableTennisV3.PresentationLayer.Views     5      O
ViewModels8clr-namespace:TableTennisV3.PresentationLayer.ViewModels     5      :Tools(clr-namespace:TableTennisV3.ToolResource     5	      -   5        $��batContorller��5      $��299.385��5
      $��444.308��60     � DataContext  5      =  7TableTennisV3.PresentationLayer.ViewModels.BatViewModel  5   
   .��5      � .+�5   
   �� $
��Left=�6   $��43��6/   $
��Wrap=�6;   $	��Top=�6P   $	��214��6h   $��
10,246,0,0q�6�   ���	#FFC5C5C5��6t   �� *
PMAC_msg+6�   �� 5   
   $
��Left=�6   $	��114��60   $��
10,127,0,0q�6=   $	��Top=�6Q   $	��214��6i   .@�5      �� $��Program mode��6   $��
-2,-2,1,-1q�6,   .��5      � ���	#FFE5E5E5.+�5      �� $��Servo ON��6   $
��Left=�60   $��23��6K   $��	10,10,0,0q�6W   $	��Top=�6j   $��80��6�   ��6�   �� *PMACServoONCmd+�� 5      $	��Run��6   $
��Left=�6+   $��23��6F   $��
100,10,0,0q�6R   $	��Top=�6f   $��83��6~   ��6�   �� *PMACRunPgmCmd+�� 5      $��	Servo OFF��6   $
��Left=�61   $��23��6L   $��	15,49,0,0q�6X   $	��Top=�6k   $��75��6�   ��6�   �� *PMACServoOFFCmd+5      5      �� 5      $��Cmd Mode��6   $��-2,-2,-12,-1q�6(   .��5      � ���	#FFE5E5E5.+�5      �� $
��Left=�6!   $��60��6<   $��10,0,0,0q�6H   $	��163��6Z   .+�5      �� $
��Left=�6"   $��24��6=    �� TextWrapping$
 Wrap=�6I   $	��Top=�6^   $	��153��6v   $��10,0,0,0q�6�   ���	#FFC5C5C5��6�   �� *
PMAC_cmd+ �� Mode$ TwoWay=�6�   �� 5      $
��Send��6!   $
��Left=�60   $��29��6K   $��10,0,0,0q�6W   $	��153��6i   ��6u   �� *SendToPMACCmd+5      5       5!      5"      �� 5$   
   $��GroupBox��6   $
��Left=�6%   $	��114��6@   $��	10,10,0,0q�6M   $	��Top=�6`   $	��214��6x   .��5%      � $��86��6   $��	0,10,0,-4q�6   $	��Top=�62   $
��Left=�6J   $	��205��6e   .+�5&      �� $
��Left=�6   $
 Wrap=�65   $��72��6m   $��20��6x   $	��Top=�6�   ��6I   �� *Hit_X+$ TwoWay=�6�   �� 5'      $
��Left=�6   $��77,0,0,0q�64   $��72��6F    � SmallChange$ 10Q�6v    � Maximum$	 550Q�6�    � Minimum$
 -550Q�6�   $��20��6�   $	��Top=�6�    � Value 6Q   �� *Hit_X+$ TwoWay=�6�   �� 5)      $
��Left=�6   $��0,25,0,0q�65   $
 Wrap=�6G   $��72��6   $��20��6�   $	��Top=�6�   ��6[   �� *Hit_Y+$ TwoWay=�6�   �� 5*      $
��Left=�6   $��	77,25,0,0q�64   $��72��6G   $ 10Q�6w   $	 250Q�6�   $
 -100Q�6�   $��20��6�   $	��Top=�6�    6R   �� *Hit_Y+$ TwoWay=�6�   �� 5,      $
 Wrap=�6   $��20��6S   $	��Top=�6_   $��
136,59,0,0q�6w   $
��Left=�6�   $��35��6�   ��6.   �� *Hit_VX+$ TwoWay=�6�   �� 5-      $
��Left=�6   $��
148,-5,0,0q�64   $��23��6H   $
 0.05Q�6y   $ 1Q�6�   $ 0Q�6�   $��59��6�   $	��Top=�6�    �� Orientation$ Vertical=�6�    6S   �� *Hit_VX+$ TwoWay=�6�   �� 5/      $
 Wrap=�6   $��20��6S   $	��Top=�6_   $��
176,59,0,0q�6w   $
��Left=�6�   $��29��6�   ��6.   �� *Hit_VY+$ TwoWay=�6�   �� 50      $
��Left=�6   $��
176,-5,0,0q�64   $��23��6H   $	 0.1Q�6y   $ 2Q�6�   $ 0Q�6�   $��59��6�   $	��Top=�6�   $ Vertical=�6�    6S   �� *Hit_VY+$ TwoWay=�6�   �� 53      $��
ConectPMAC��6   $
��Left=�6.   $��20��6I   $��0,59,0,0q�6U   $	��Top=�6g   $	��131��6   ��6�   �� *ConectToPMACCmd+54      55      �� 56   
   $��GroupBox��6   $
��Left=�6%   $	��132��6@   $��
229,10,0,0q�6M   $	��Top=�6a   $	��206��6y   .��57      � $
��Left=�6   $��93��6.   $��10,10,-2,-1q�6:   $	��Top=�6O   $	��191��6g   .+�58      �� $
��Left=�6   $
 Wrap=�6G   $��72��6   $��20��6�   $	��Top=�6�   ��6[   �� *Hit_Z+$ TwoWay=�6�   �� 59      $
��Left=�6   $��0,25,0,0q�65   $
 Wrap=�6G   $��72��6   $��20��6�   $	��Top=�6�   ��6[   �� *Hit_S+$ TwoWay=�6�   �� 5:      $
��Left=�6   $��0,50,0,0q�65   $
 Wrap=�6G   $��72��6   $��20��6�   $	��Top=�6�   ��6[   �� *Hit_P+$ TwoWay=�6�   �� 5;      $
��Left=�6   $��83,0,0,0q�64   $��72��6G   $	 100Q�6w   $	 430Q�6�   $	 240Q�6�   $��20��6�   $	��Top=�6�    6R   �� *Hit_Z+$ TwoWay=�6�   �� 5<      $
��Left=�6   $��	83,25,0,0q�64   $��72��6G   $	 100Q�6w   $	 165Q�6�   $ 80Q�6�   $��20��6�   $	��Top=�6�    6R   �� *Hit_S+$ TwoWay=�6�   �� 5=      $
��Left=�6   $��	83,50,0,0q�64   $��72��6G   $	 100Q�6w   $	 160Q�6�   $	 110Q�6�   $��20��6�   $	��Top=�6�    6R   �� *Hit_P+$ TwoWay=�6�   �� 5>      $��	ConectZSP��6   $
��Left=�6-   $��24��6H   $��	0,75,0,-6q�6T   $	��Top=�6g   $	��181��6    J� RenderTransformOrigin$ 
0.66,1.208&�6�   ��6�   �� *ConectToZSPCmd+6�   5?      5@      � 5B   
   $��229,147,10,10q�6   .+�5C      �� $��Connect To Vision��6   $��29��61   $	��Top=�6=   $��0,0,4,0q�6U   ��6f   �� *ConectToVisionCmd+�� 5D      $��0,34,0,0q�6   $
��Wrap=�6*   ��6>   �� 	 �� Path$	 VisionResult�5E      5G      5H      