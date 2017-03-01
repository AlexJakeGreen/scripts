package ua.kh.bars.services;

import ua.kh.bars.entities.Event;
import ua.kh.bars.entities.Ticket;
import ua.kh.bars.entities.User;

import java.util.Date;
import java.util.List;

/**
 * Created by green on 3/18/16.
 */
public interface TicketService {

    List<Ticket> findAll();
    Ticket createTicket(User user, Event event, int seat, Date date, int price);
    void save(Ticket ticket);

}
