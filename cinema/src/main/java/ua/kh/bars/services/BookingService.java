package ua.kh.bars.services;

import ua.kh.bars.entities.Event;
import ua.kh.bars.entities.Ticket;
import ua.kh.bars.entities.User;

/**
 * Created by green on 3/28/16.
 */
public interface BookingService {

    Ticket bookTicket(Event event, User user, int seat);

}
