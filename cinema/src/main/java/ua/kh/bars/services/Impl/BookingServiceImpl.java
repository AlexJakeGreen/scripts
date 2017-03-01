package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import ua.kh.bars.entities.Event;
import ua.kh.bars.entities.Ticket;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.*;

/**
 * Created by green on 3/28/16.
 */

@Service
@Transactional(rollbackFor = Exception.class)
public class BookingServiceImpl implements BookingService {

    @Autowired
    EventService eventService;

    @Autowired
    UserService userService;

    @Autowired
    TicketService ticketService;

    @Autowired
    UserAccountService userAccountService;

    public Ticket bookTicket(Event event, User user, int seat) {
        Ticket ticket = ticketService.createTicket(user, event, seat, event.getDate(), event.getPrice());
        user.setTickets(user.getTickets() + 1);
        if(userAccountService.withdraw(user, event.getPrice())) {
            ticketService.save(ticket);
            userService.save(user);
            return ticket;
        };
        return null;
    }
}
