/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: target.h
**创   建   人: 陈明计
**最后修改日期: 2003年5月30日
**描        述: lpc210x（飞利浦的ARM）目标板特殊的代码头文件
**              每个工程应当具有这个文件的拷贝，用户根据程序的需要修改本文件
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: v1.0
** 日　期: 2003年5月30日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人:
** 版  本:
** 日　期:
** 描　述:
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef IN_TARGET 

extern void Reset(void);
/*********************************************************************************************************
** 函数名称: Reset
** 功能描述: 目标板软复位
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2003年5月30日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

extern void TargetInit(void);
/*********************************************************************************************************
** 函数名称: TargetInit
** 功能描述: 目标板初始化代码，在需要的地方调用，根据需要改变
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2003年5月30日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/


#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
