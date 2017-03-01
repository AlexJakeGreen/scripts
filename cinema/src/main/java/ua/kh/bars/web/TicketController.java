package ua.kh.bars.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import ua.kh.bars.entities.Ticket;
import ua.kh.bars.services.TicketService;

/**
 * Created by green on 3/19/16.
 */

@Controller
@RequestMapping("/tickets")
public class TicketController {

    @Autowired
    TicketService ticketService;

    @RequestMapping(method = RequestMethod.GET)
    public String indexTickets(Model model) {
        Iterable<Ticket> tickets = ticketService.findAll();
        model.addAttribute("tickets", tickets);
        return "tickets/index";
    }
}
