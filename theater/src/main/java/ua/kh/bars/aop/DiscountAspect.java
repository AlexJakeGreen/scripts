package ua.kh.bars.aop;

import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.JoinPoint;

import java.util.Map;
import java.util.HashMap;
import java.util.Date;
import ua.kh.bars.entity.User;
import ua.kh.bars.entity.Ticket;


@Aspect
public class DiscountAspect {

    private Map<String, Integer> discountsPerType = new HashMap<>();
    private Map<User, Integer> discountsPerUser = new HashMap<>();
        
    @Pointcut("execution(* *..DiscountStrategy.getDiscount(..))")
    private void discountsCounter() {
    }

    @AfterReturning("discountsCounter() && args(user, event, date)")
    public void countDiscounts(JoinPoint joinPoint, User user, Object event, Date date) {
        // Per discount
        if (!discountsPerType.containsKey(joinPoint.getTarget().getClass().getSimpleName())) {
            discountsPerType.put(joinPoint.getTarget().getClass().getSimpleName(), 0);
        }
        discountsPerType.put(joinPoint.getTarget().getClass().getSimpleName(),
                             discountsPerType
                             .get(joinPoint.getTarget().getClass().getSimpleName()) + 1);
        // Per user
        if (!discountsPerUser.containsKey(user)) {
            discountsPerUser.put(user, 0);
        }
        discountsPerUser.put(user, discountsPerUser.get(user) + 1);
    }

    public Map<String, Integer> getDiscountsPerTypeCount() {
        return discountsPerType;
    }

    public Map<User, Integer> getDiscountsPerUser() {
        return discountsPerUser;
    }

}
