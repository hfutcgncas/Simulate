close all
load X0.txt
M = X0;

M = sortrows(M,4);
for i = 1:size(M,1)
    if M(i,7) == 1
        plot3(M(i,1),M(i,2),M(i,3),'r.');
        hold on
    elseif M(i,7) == 2
        plot3(M(i,1),M(i,2),M(i,3),'b.');
        hold on
    elseif M(i,7) == 3
        plot3(M(i,1),M(i,2),M(i,3),'g.');
        hold on
    elseif M(i,7) == 4
        plot3(M(i,1),M(i,2),M(i,3),'y.');
        hold on
    end
end
plot3(M(:,1),M(:,2),M(:,3),'b.');
xlabel('X');ylabel('Y');zlabel('Z');

hold off
P = M(:,1:3)';
P(1,:) =  P(1,:) - 450;
P(3,:) =  P(3,:) - 15;
P = [P; ones(1,size(P,2))];

    Cam89Ex = [-0.325034    0.935383    -0.139328   -548.541591;
                0.476724    0.034820    -0.878363   -438.7979;
                -0.816754   -0.351919   -0.457237   4174.397803;
                0           0           0           1           ];

    Cam89In = [1122.59606   0           303.38838   0;
                0           1125.18739  244.66280   0;
                0           0           1           0];
            

    Cam88Ex = [ 0.579236    0.791127    0.196477    -1036.398277;
                0.432601    -0.094049   -0.896667   -243.499825;
                -0.690899   0.604378    -0.396719   3683.331526;
                0           0           0            1          ];

    Cam88In = [1127.36285   0           358.68825   0;
                0           1130.94781  206.88433   0;
                0           0           1           0];
            
            
Cam98In = [1050.77334     0               466.4264     0;
               0         1148.63287       410.29566    0;
               0          0                1           0];
           
 Cam98Ex = [0.183304	-0.980819	-0.066284	-290.643287;
            -0.719003	-0.087781	-0.689441	32.01393000;
            0.670398	0.174036	-0.721303	2148.094319;
            0	          0	          0	             1];

% Cam98Ex =  [0.245966    -0.959237   -0.139158   -79.910329;
%            -0.650458    -0.056911   -0.757407   516.967916;
%             0.718613    0.276813    -0.637942   2280.513203
%             0             0               0       1];
        
 %Cam98Ex(:,4)=[     -31.966379 105.245466 2136.745538 1]';
 %Cam98Ex(:,4)=[     -44.170985 140.235112 2098.654999 1]';
% Cam98Ex(:,4)=[     -92.063223 135.168745 2109.695645 1]';
   
 p =      Cam98In*  Cam98Ex*P;
 for i = 1:size(p,2)
     p(:,i)=p(:,i)/p(3,i);
 end
 figure,
 plot(p(1,:),-p(2,:),'r.');
axis([0,400,-300,0]);
 
  p =      Cam88In*  Cam88Ex*P;
 for i = 1:size(p,2)
     p(:,i)=p(:,i)/p(3,i);
 end
 figure,
 plot(p(1,:),-p(2,:),'b.');
axis([0,400,-300,0]);
 
   p =      Cam89In*  Cam89Ex*P;
 for i = 1:size(p,2)
     p(:,i)=p(:,i)/p(3,i);
 end
 figure,
 plot(p(1,:),-p(2,:),'g.');
axis([0,400,-300,0]);


P1 = [0 0 0 1 ]';
 p1 =      Cam98In*  Cam98Ex*P1;
 for i = 1:size(p1,2)
     p1(:,i)=p1(:,i)/p1(3,i);
 end
 p98 = p1;

 
  p1 =      Cam88In*  Cam88Ex*P1;

 for i = 1:size(p1,2)
     p1(:,i)=p1(:,i)/p1(3,i);
 end
   p88 = p1;

 
   p1 =      Cam89In*  Cam89Ex*P1;
 for i = 1:size(p1,2)
     p1(:,i)=p1(:,i)/p1(3,i);
 end


 p89 = p1;