package ua.kh.bars;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Date;
import java.util.Map;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import ua.kh.bars.entity.Auditorium;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.EventRating;
import ua.kh.bars.entity.Ticket;
import ua.kh.bars.entity.User;
import ua.kh.bars.service.AuditoriumService;
import ua.kh.bars.service.BookingService;
import ua.kh.bars.service.DiscountService;
import ua.kh.bars.service.EventService;
import ua.kh.bars.service.UserService;

import ua.kh.bars.aop.CounterAspect;

public class App {

    private ConfigurableApplicationContext ctx;

    public App() {}

    private static void registerUsers(UserService service) {
        service.register("ajgreen@gmail.com", "Alex", fromString("06-04-1986"));
        service.register("TheMoonLightCat@natalie.org", "Kuzya", fromString("09-09-2011"));
        service.register("test@example.org", "Test User", fromString("06-02-2016"));
    }

    private static void createEvents(EventService service) {
        service.create(fromString("06-04-2016 20:40"), "Terminator 1", 123, EventRating.HIGH);
        service.create(fromString("07-04-2016 16:45"), "Chronicles of Shannara", 99, EventRating.MEDIUM);
        service.create(fromString("08-04-2016 18:30"), "How I met your mom", 45, EventRating.LOW);
    }

    private static void bookTicket(EventService eventService,
                                   UserService userService,
                                   BookingService bookingService) {
        Event event = eventService.getById(fromString("06-04-2016 20:40"));
        User user = userService.getUserByEmail("test@example.org");
        int ticketPrice = bookingService.getTicketPrice(event, event.getDate(), 40, user);
        bookingService.bookTicket(user, new Ticket(user, event, 0, 10, event.getDate()));
        bookingService.bookTicket(user, new Ticket(user, event, 0, 67, event.getDate()));
    }

    public static void main( String[] args ) {
        ConfigurableApplicationContext ctx = new ClassPathXmlApplicationContext("spring.xml");
        App app = (App)ctx.getBean("app");
        UserService userService = (UserService)ctx.getBean("userService");
        AuditoriumService auditoriumService = (AuditoriumService)ctx.getBean("auditoriumService");
        DiscountService discountService = (DiscountService)ctx.getBean("discountService");
        EventService eventService = (EventService)ctx.getBean("eventService");
        BookingService bookingService = (BookingService)ctx.getBean("bookingService");

        // Moved long blocks to separate methods
        // just to make the main method more readable
           registerUsers(userService);
           createEvents(eventService);
           

        eventService.assignAuditorium(eventService.getAll().iterator().next(),
                                      auditoriumService.getAuditoriums().iterator().next(),
                                      fromString("09-04-2016 18:00"));
        

        bookTicket(eventService, userService, bookingService);
    
        System.out.println("");
        Event event = eventService.getById(fromString("06-04-2016 20:40"));
        Iterable<Ticket> tickets = bookingService.getTicketsForEvent(event, event.getDate());
        
        Iterable<Event> events = eventService.getByName("Terminator 1");
        CounterAspect counterAspect = (CounterAspect)ctx.getBean("counterAspect");
        System.out.println("Total users in DB: " + userService.getCount());

        counterAspect.printCounters();
        
    }

    /*
      Input format: dd-MM-yyy HH:mm
      Return value: Date or null if wrong input
      In UI we have to ask user again after he entered date in wrong format
     */
    private static Date fromString(String plainDate) {
        SimpleDateFormat df = new SimpleDateFormat("dd-MM-yyyy HH:mm");
        if(plainDate.length() == 10) {
            plainDate += " 00:00";
        }
        Date result = null;
        try {
            result = df.parse(plainDate);
        } catch (ParseException e) {
        }
        return result;
    }
}
