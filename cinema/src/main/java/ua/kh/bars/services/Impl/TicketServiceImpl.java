package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import ua.kh.bars.entities.Event;
import ua.kh.bars.entities.Ticket;
import ua.kh.bars.entities.User;
import ua.kh.bars.repositories.TicketRepository;
import ua.kh.bars.services.TicketService;

import java.util.Date;
import java.util.List;

/**
 * Created by green on 3/18/16.
 */

@Service
@Transactional
public class TicketServiceImpl implements TicketService {

    @Autowired
    private TicketRepository ticketRepository;

    public List<Ticket> findAll() {
        return ticketRepository.findAll();
    }

    public Ticket createTicket(User user, Event event, int seat, Date date, int price) {
        Ticket ticket = new Ticket();
        ticket.setUser(user);
        ticket.setEvent(event);
        ticket.setSeat(seat);
        ticket.setDate(event.getDate());
        ticket.setPrice(event.getPrice());
        return ticket;
    }

    public void save(Ticket ticket) {
        ticketRepository.saveAndFlush(ticket);
    }
}
