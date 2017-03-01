package ua.kh.bars.endpoints;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ws.server.endpoint.annotation.Endpoint;
import org.springframework.ws.server.endpoint.annotation.PayloadRoot;
import org.springframework.ws.server.endpoint.annotation.RequestPayload;
import org.springframework.ws.server.endpoint.annotation.ResponsePayload;
import ua.kh.bars.entities.Event;
import ua.kh.bars.services.AuditoriumService;
import ua.kh.bars.services.EventService;
import ua.kh.bars.soap.FindEventByIdRequest;
import ua.kh.bars.soap.FindEventByIdResponse;

/**
 * Created by green on 4/13/16.
 */

@Endpoint
public class EventEndpoint {

    private static final String NS_URI = "http://bars.kh.ua/soap";

    @Autowired
    EventService eventService;

    @Autowired
    AuditoriumService auditoriumService;

    @PayloadRoot(namespace = NS_URI, localPart = "FindEventByIdRequest")
    @ResponsePayload
    public FindEventByIdResponse findEventById(@RequestPayload FindEventByIdRequest request) {
        Event event = eventService.findOne(request.getId());
        FindEventByIdResponse response = new FindEventByIdResponse();
        // The second approach of mapping JAXB to Domain
        // (see UserEndpoint.java for the first one)
        ua.kh.bars.soap.Event jaxbEvent = new ua.kh.bars.soap.Event();
        jaxbEvent.setAuditoriumId(event.getAuditorium().getId());
        jaxbEvent.setName(event.getName());
        jaxbEvent.setId(event.getId());
        jaxbEvent.setPrice(event.getPrice());
        jaxbEvent.setImageUrl(event.getImage_url());
        jaxbEvent.setDate(event.getDate().toString());
        response.setEvent(jaxbEvent);
        return response;
    }

}
