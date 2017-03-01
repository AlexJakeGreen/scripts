package ua.kh.bars.aop;

import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.JoinPoint;

import java.util.Map;
import java.util.HashMap;
import java.util.Date;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;
import ua.kh.bars.entity.Ticket;
import ua.kh.bars.dao.CounterDAO;

@Aspect
public class CounterAspect {

    private CounterDAO counterDAO;

    CounterAspect(CounterDAO counterDAO) {
        this.counterDAO = counterDAO;
    }
    
    @Pointcut("execution(* *..EventService.getByName(..))")
    private void searchEventsByNameCounter() {
    }

    @Pointcut("execution(* *..BookingService.getTicketPrice(..))")
    private void getEventPriceCounter() {
    }

    @Pointcut("execution(* *..BookingService.bookTicket(..))")
    private void bookTicketCounter() {
    }

    @AfterReturning(pointcut="searchEventsByNameCounter()", returning="events")
    public void countEventAccess(JoinPoint joinPoint, Iterable<Event> events) {
        for(Event event : events) {
            int searches = counterDAO.getCount("searchEventsByName", event);
            counterDAO.saveCounter("searchEventsByName", event, searches + 1);
        }
    }

    @AfterReturning("getEventPriceCounter() && args(event, date, seat, user)")
    public void countEventTicketPriceAccess(JoinPoint joinPoint, Event event, Date date, int seat, User user) {
            int searches = counterDAO.getCount("getEventPrice", event);
            counterDAO.saveCounter("getEventPrice", event, searches + 1);
    }

    @AfterReturning("bookTicketCounter() && args(user, ticket)")
    public void countEventTicketPriceAccess(JoinPoint joinPoint, User user, Ticket ticket) {
        int searches = counterDAO.getCount("bookTicket", ticket.getEvent());
        counterDAO.saveCounter("bookTicket", ticket.getEvent(), searches + 1);
    }

    public void printCounters() {
        counterDAO.printCounters();
    }

}
