package ua.kh.bars.aop;

import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.ProceedingJoinPoint;

import java.util.Map;
import java.util.HashMap;
import java.util.Date;
import ua.kh.bars.entity.User;
import ua.kh.bars.entity.Ticket;


@Aspect
public class LuckyWinnerAspect {

    @Pointcut("execution(* *..BookingService.bookTicket(..))")
    private void winLuckyTicket() {
    }

    @Around("winLuckyTicket() && args(user, ticket)")
    public void bookTicket(ProceedingJoinPoint jp, User user, Ticket ticket) throws Throwable{
        jp.proceed();
        if(user.isLucky()) {
            ticket.setPrice(0);
            user.setSystemMessage("You're lucky");
        }
    }
   
}
