package ua.kh.bars.service;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import ua.kh.bars.dao.TicketDAO;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.EventRating;
import ua.kh.bars.entity.Ticket;
import ua.kh.bars.entity.User;
import ua.kh.bars.service.DiscountService;
import ua.kh.bars.service.UserService;

public class BookingService {
    DiscountService discountService;
    TicketDAO ticketDAO;
    UserService userService;
    double vipMult;
    double highRateMult;

    public BookingService() {
    }

    public BookingService(DiscountService discountService,
                          TicketDAO ticketDAO,
                          UserService userService,
                          double vipMult,
                          double highRateMult) {
        this.discountService = discountService;
        this.ticketDAO = ticketDAO;
        this.userService = userService;
        this.vipMult = vipMult;
        this.highRateMult = highRateMult;
    }

    public int getTicketPrice(Event event, Date date, int seats, User user) {
        double price = (double)event.getPrice();
        boolean vip = false;
        if (event.getAuditorium() != null &&
            event.getAuditorium().getVipSeats() != null) {
            for (String seat : event.getAuditorium().getVipSeats()) {
                if (Integer.parseInt(seat) == seats && seats > 0) {
                    vip = true;
                }
            }
        }
        if (vip == true) {
            price = price * vipMult;
        }
        if (event.getRating() == EventRating.HIGH) {
            price = price * highRateMult;
        }
        int discount = discountService.getDiscount(user, event, date);
        price = price * (1 - (discount / 100));
        return  (int)price;
    }

    public void bookTicket(User user, Ticket ticket) {
        if (user.getId() != null) {
            ticket.setUser(user);
            int bookedTickets = user.getTickets();
            user.setTickets(bookedTickets + 1);
        }
        int price = getTicketPrice(ticket.getEvent(), ticket.getDate(),
                                   ticket.getSeat(), user);
        ticket.setPrice(price);
        ticketDAO.save(ticket);
    }

    public Iterable<Ticket> getTicketsForEvent(Event event, Date date) {
        return ticketDAO.searchByEventDate(event, date);
    }
}
