package ua.kh.bars.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import ua.kh.bars.entities.Event;
import ua.kh.bars.entities.Ticket;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.BookingService;
import ua.kh.bars.services.EventService;
import ua.kh.bars.services.UserService;

/**
 * Created by green on 4/17/16.
 */

@RestController
@RequestMapping("/rest/bookTicket")
public class BookingRestController {

    @Autowired
    BookingService bookingService;

    @Autowired
    EventService eventService;

    @Autowired
    UserService userService;

    @RequestMapping(value = "/", method = RequestMethod.GET)
    public Ticket bookTicket(@RequestParam("eventId") Long eventId) {
        Event event = eventService.findOne(eventId);
        User user = userService.findOne(1L);
        Ticket ticket = bookingService.bookTicket(event, user, 5);
        return ticket;
    }
}