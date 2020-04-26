#include <reg52.h>

sbit p34=P3^4;
sbit p35=P3^5;
sbit p36=P3^6;
sbit p37=P3^7;
sbit pe1=P2^4;

sbit p30=P3^0;
sbit p31=P3^1;
sbit p32=P3^2;
sbit p33=P3^3;
sbit pe2=P2^5;
		  
sbit p10=P1^0;
sbit p11=P1^1;
sbit p12=P1^2;
sbit p13=P1^3;
sbit pup=P2^6;

sbit p14=P1^4;
sbit p15=P1^5;
sbit p16=P1^6;
sbit p17=P1^7;
sbit pdown=P2^7;


//EleOne ��ʾ����1���ڴ��ڵ�¥�㣬EleTwo��ʾ���ڵ���2���ڵ�¥��
//EleOneState��ʾ����1������״̬������0��ֹ��1���ϣ�2����
//EleTwoState��ʾ����2������״̬�����ֵĺ����EleOneState��ȫ��ͬ
int EleOne=1,EleTwo=1,EleOneState=0,EleTwoState=0;
int e1=0,e2=0,down=0,up=0;


void delay(int k){
	int i;
	for(i=0;i<k;i++){}
}


void display_LED(){

	int i=0;
	//���ε����˸���
	while(i<8){
	  P2=i;
	  pe1=(e1>>i)&1==1;//�����iλΪ1�����Ӧ�ĵ���
	  pdown=(down>>i)&1==1;
	  pup=(up>>i)&1==1;
	  pe2=(e2>>i)&1==1;
	  i++;
	  delay(50);
	}

}

void display_Elevator(){

	P0=EleTwo+EleOne*16;
}

//���񰴼�
void update_click(){
	//e1���ݰ���
		if(p37){
			e1|=(1<<((p36*4)|(p35*2)|(p34)));
		}
	//down���ݰ�ť
		if(p17){
			down|=(1<<((p16*4)|(p15*2)|(p14)));
		}
	//up�İ�ť
		if(p13){
			up|=(1<<((p12*4)|(p11*2)|(p10)));
		}
	//e2��ť
		if(p33){
			e2|=(1<<((p32*4)|(p31*2)|(p30)));
		}
}
int downFloor(int *x,int *y){

    if((*x&(~((*x>>(*y-1))<<(*y-1))))>0){
        return 1;
    }else{
        return 0;
    }

}


void update_LED(){

	if(EleOneState==1){
		//��������״̬
		up&=~(1<<(EleOne-1));
		e1&=~(1<<(EleOne-1));//��EleOne����Ϊ0
		//��Ϊ�˽ӵ����¥���������Ա�����ڵ�����״̬,���ҵ�ǰ¥����������Ա�е���߲�
		if((e1>>EleOne)<=0&&(up>>EleOne)<=0&&(down>>EleOne<=0)){
			down&=~(1<<(EleOne-1));
		}
	
	}else if(EleOneState==2){

		down&=~(1<<(EleOne-1));
		e1&=~(1<<(EleOne-1));
		//Ϊ�˽ӵ����¥���������Ա,���ҵ�ǰ¥����������Ա�е���ײ�
		if(downFloor(&e1,&EleOne)<=0&&downFloor(&down,&EleOne)<=0&&downFloor(&up,&EleOne)<=0){
			up&=~(1<<(EleOne-1));
		}
	}


	if(EleTwoState==1){
        //��������״̬
        up&=~(1<<(EleTwo-1));
        e2&=~(1<<(EleTwo-1));//��EleTwo����Ϊ0
        //��Ϊ�˽ӵ����¥���������Ա�����ڵ�����״̬,���ҵ�ǰ¥����������Ա�е���߲�
        if((e1>>EleTwo)<=0&&(up>>EleTwo)<=0&&(down>>EleTwo<=0)){
            down&=~(1<<(EleTwo-1));
        }

    }else if(EleTwoState==2){

        down&=~(1<<(EleTwo-1));
        e2&=~(1<<(EleTwo-1));
        //Ϊ�˽ӵ����¥���������Ա,���ҵ�ǰ¥����������Ա�е���ײ�
        if(downFloor(&e1,&EleTwo)<=0&&downFloor(&down,&EleTwo)<=0&&downFloor(&up,&EleTwo)<=0){
            up&=~(1<<(EleTwo-1));
        }
    }
}


//Look���ݵ����㷨
void LookAlgorithm()
{
    //����e1
    if(EleOneState==0){
        if((e1>>EleOne)>0){
            EleOneState=1;
            EleOne++;
        }else if(downFloor(&e1,&EleOne)){
            EleOneState=2;
            EleOne--;
        }

    }else if(EleOneState==1){
        //��ʱ���ݴ������Ϲ���״̬����ֻҪ�����ڲ��������ϻ��߸�¥�����˰�������
        if((e1>>EleOne)>0||(up>>EleOne)>0){
            EleOne++;
        }else if((down>>EleOne)>0){
            //��¥�㰴�����£������ϵ����¥��������
            EleOne++;
        }else if(downFloor(&e1,&EleOne)||downFloor(&up,&EleOne)||downFloor(&down,&EleOne)){
            //�������ڲ�����Ҫ���϶��Ҹ�¥���ް�ť���£��鿴��¥��
            EleOneState=2;
            EleOne--;
        }else{
            EleOneState=0;
        }
    }else{
        if(downFloor(&e1,&EleOne)||downFloor(&down,&EleOne)){
            //��������״̬ʱ����������ڲ���Ҫ���»��ߵ�¥�㰴�����°�ť
            EleOne--;
        }else if(downFloor(&up,&EleOne)){
            //��¥���������ϣ��������¥��������
            EleOne--;
        }else if((e1>>EleOne)>0||(up>>EleOne)>0||(down>>EleOne)>0){
            //��Ӧ��¥�������
            EleOneState=1;
            EleOne++;
        }else{
            EleOneState=0;
        }

    }

    //����e2
    if(EleTwoState==0){
        if((e2>>EleTwo)>0){
            EleTwoState=1;
            EleTwo++;
        }else if(downFloor(&e2,&EleTwo)){
            EleTwoState=2;
            EleTwo--;
        }

    }else if(EleTwoState==1){
        //��ʱ���ݴ������Ϲ���״̬����ֻҪ�����ڲ��������ϻ��߸�¥�����˰�������
        if((e2>>EleTwo)>0||(up>>EleTwo)>0){
            EleTwo++;
        }else if((down>>EleTwo)>0){
            //��¥�㰴�����£������ϵ����¥��������
            EleTwo++;
        }else if(downFloor(&e2,&EleTwo)||downFloor(&up,&EleTwo)||downFloor(&down,&EleTwo)){
            //�������ڲ�����Ҫ���϶��Ҹ�¥���ް�ť���£��鿴��¥��
            EleTwoState=2;
            EleTwo--;
        }else{
            EleTwoState=0;
        }
    }else{
        if(downFloor(&e2,&EleTwo)||downFloor(&down,&EleTwo)){
            //��������״̬ʱ����������ڲ���Ҫ���»��ߵ�¥�㰴�����°�ť
            EleTwo--;
        }else if(downFloor(&up,&EleTwo)){
            //��¥���������ϣ��������¥��������
            EleTwo--;
        }else if((e2>>EleTwo)>0||(up>>EleTwo)>0||(down>>EleTwo)>0){
            //��Ӧ��¥�������
            EleTwoState=1;
            EleTwo++;
        }else{
            EleTwoState=0;
        }
    }

    if(EleOneState==0&&EleTwoState==0){
        //�������ݶ���ֹ
        if(EleOne>EleTwo){
            //����1ֹͣ��¥��ȵ���2�ߣ������1�����¥�㣬����2�����¥��
            if((up>>EleOne)>0||(down>>EleOne)>0){
                EleOneState=1;
                EleOne++;
            }

            if(downFloor(&up,&EleTwo)||downFloor(&down,&EleTwo)){
                EleTwoState=2;
                EleTwo--;
            }
            if(EleOneState==0&&EleTwoState==0){
                if(up!=0||down!=0){
                    //������¥���м�
                    EleOne--;
                    EleOneState=2;
                    EleTwo++;
                    EleTwoState=1;
                }
            }
        }else{

            if((up>>EleTwo)>0||(down>>EleTwo)>0){
                EleTwoState=1;
                EleTwo++;
            }

            if(downFloor(&up,&EleOne)||downFloor(&down,&EleOne)){
                EleOneState=2;
                EleOne--;
            }

            if(EleOneState==0&&EleTwoState==0){
                if(up!=0||down!=0){
                    //������¥���м�
                    EleOne++;
                    EleOneState=1;
                    EleTwo--;
                    EleTwoState=2;
                }
            }
        }
    }else if(EleOneState==0){
            //����1��ֹ
            if(EleTwoState==1){
                //���ݶ�������
                if(downFloor(&up,&EleOne)){
                    //���ҵ���2����Ҳ������,�����2�����¥������
                    EleOne--;
                    EleOneState=2;
                }
            }else{
                if((down>>EleOne)>0){
                    EleOne++;
                    EleOneState=1;
                }
            }

    }else if(EleTwoState==0){

            if(EleOneState==1){
                if(downFloor(&up,&EleTwo)){
                    EleTwo--;
                    EleTwoState=2;
                }
            }else{
                if((down>>EleTwo)>0){
                    EleTwo++;
                    EleTwoState=1;
                }
            }
        }

        update_LED();

}


void main()
{
	int i;
 	while(1){
		i=0;
		display_LED();
		display_Elevator();
		update_click();
		LookAlgorithm();
		display_Elevator();
		
		while(i<120){
			update_click();
			display_LED();
			i++;
		}
	}
}